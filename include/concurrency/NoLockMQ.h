/*
author: YiliangWu
简介：线程间通信用无锁消息队列。
	一个队，只支持一个线程写，一个线程读。 需要相互通讯，分配2个消息队列就可以了。
	队列可用空间固定大小。 先简化问题，以后再考虑动态增长。
	队列快满了生产者就限时阻塞。 
	队列空了消费者就忽略跳过，继续跑别的逻辑。 

设计思想：
	    队列满了，不能就让生产者一直阻塞等可写。如果这样做，会容易产生生产者和消费者互相死锁。 比如：
		{
			A,B两个线程互相通讯，有2 队列AB 队列BA.
			A生产 --队列AB--> B消费
			A消费 <--队列BA-- B生产

			某个时刻发生 队列AB 队列BA 都满了的情况，就死锁了。
		}
		解决方法1：
		{
			队列无限增长空间。 
			缺点：如果逻辑设计不合理导致生产者一直比消费者快，导致内存无限增长就等挂了。
		}
		解决方法2：
		{
			队列空间有限。 检测快满了就让生产者放慢速度（限时阻塞）。
			缺点：生产者放慢速度。可能的事情总会发生的，设计极端不合理（比如消费者设计有大缺陷，一直很慢），导致队列满了，无法写入。数据就丢失。
		}
		这里选用 解决方法2，遇到队列满的情况，就出严重错误日志，停掉进程。去修复你的线程设计问题吧，而不是修复消息队列问题。


代码设计：
	thread:
	消息队列固定大小。
	使用90%算快满状态。
	两个指针，读指针，写指针。 写指针只能写线程修改， 。。

	伪代码：
	{
		void write(msg, len)
		{
			if(deque.will_full())//快满了，就先歇歇，等下消费者。
				block( second);
			if(deque.full(msg)) //写入msg就满了
			{
				LogFatal("你的设计有重大缺陷，去改你的设计");
				exit();
			}
			deque.write(msg); //先写数据，再写写指针，不需要加锁

		}

		const char *read(int &len)
		{
			if(deque.empty())
				return
			deque.read(msg);//先读数据，再写读指针，不需要加锁
		}

	}

*/

#pragma once

#include "singleton.h"
#include "typedef.h"
#include "cnt_typedef.h"
#include <thread>

namespace su
{

	class NoLockMQ
	{
	private:
		int WriteThreadId; //调试版本用，用户错误调用就出日志
		int ReadThreadId;
		char *write;
		char *read;
		char buf[1024];
	public:
		void IsWillFull();
		void Start();
		void Stop();
	};

}


//end file
