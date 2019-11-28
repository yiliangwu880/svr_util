/*
author: YiliangWu
设计：
	2线程通讯。 
	读写2无锁环形队列
	无消息挂起，用条件变量 唤醒


	条件变量写法说明：

	{
		// wait端
		pthread_mutex_lock(mtx);
		while(deque.empty())
			pthread_cond_wait(...);
		deque.pop_front();
		pthread_mutex_unlock(mtx);

		// signal端
		pthread_mutex_lock(mtx);
		deque.push_back(x);
		pthread_cond_signal(...);
		pthread_mutex_unlock(mtx);

		条件变量相关的 flag变量， 必须在锁里面才能保证正确。这里为deque
	}

	thread:
	消息队列，消息数据为不固定大小字节。但通道，也就是一个写线程，  一个读线程。
	两个指针，读指针，写指针。 写只能写线程修改， 。。

	write
	{

		lock write
		{
			while(deque.full())
				wait(...);
		}
		deque.write(msg); //先写数据，再写写指针，不需要加锁

		lock read
		{
			signal();
		}
	}

	read
	{
		
		lock read
		{
			while(deque.empty())
				wait(...);
			signal();
		}
		deque.read(msg);//先读数据，再写读指针，不需要加锁

		lock write
		{
			signal();
		}
	}

*/

#pragma once

#include "singleton.h"
#include "typedef.h"
#include "cnt_typedef.h"
#include <thread>

/*
If a detached thread uses a global or static object, you should do one of the following:
– Ensure that these global/static objects are not destroyed before all detached threads accessing
themarefinished(or finishedaccessing them). Oneapproachtoensurethis is touse condition
variables (see Section 18.6, page 1003), which the detached threads use to signal that they
have finished. Before leaving main() or calling exit(), you’d have to set these condition
variables then to signal that a destruction is possible. 9
– End the program by calling quick_exit(), which was introduced exactly for this reason to
end a program without calling the destructors for global and static objects (see Section 5.8.2,
page 162).
*/

class SuThread
{
public:

	void SetValue();
	void Start();
	void Stop();
private:
};


//end file
