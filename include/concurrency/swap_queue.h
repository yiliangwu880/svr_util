/*
实现无锁消息队列：
{
合适场景：一线程写一线程读，无锁
结构：
{
读队列指针(R)：指向任意队列对象，比如vector
写队列指针(W)：指向任意队列对象，比如vector
读队列可读控制位(C)： true表示可以读取R数据。false表示R无数据可读。
}
操作:
读线程：
{

if(C== false) idle
else(C == true) read
if(have read all R) C=false
}
写线程：
{
一直能写W
  定时调用或者写时调用：
  {
	if(C== false && W 有数据)  swap(R,W,) C=true
  }
  如果写队列位空，关闭定时器
}
}
*/
#pragma once

#include "typedef.h"
#include <string>



class SwapQueue
{
public:
	SwapQueue();

	void Write(const char *data, uint32 len);
	void OnTimer(); //定时检查，交换数据给读线程
	bool IsNeedTimer(); //return true表示需要调用OnTimer

	//分割先，上面时写线程使用，下面时读。用错就完了
	//////////////////////////////////////////
	//@data 数据被写入地址
	//@#len @data最大长度 #读取长度, 
	//失败len==0
	void Read(char *data, uint32 &len);

	//无拷贝读取
	//#data 数据地址，无数据返回nullptr
	//#len  数据长度
	//注意：必须和 drain函数配合使用。
	void peek(char *(&data), uint32 &len) {};
	//丢弃部分可读数据
	void drain(uint32 len) {};
private:
	std::string m_queue_a;
	std::string m_queue_b;
	std::string *m_pread = nullptr;
	std::string *m_pwrite = nullptr;
	bool m_is_reading = false; //true表示可以读取R数据。false表示R无数据可读。
	size_t m_read_pos=0;  //下次要读取 *m_pread的索引
};

