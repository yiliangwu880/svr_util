/*
author: YiliangWu
设计：
	2线程通讯。 
	读写2无锁环形队列
	无消息挂起，用条件变量 唤醒


	条件变量写法说明：

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

	thread:

	dispach
	{
		
		msg=null
		lock
		{
			while(deque.empty())
				pthread_cond_wait(...);
			msg = deque.get();
			pthread_cond_signal(&deque);
		}
		dispach msg
	}

	send msg
	{

		lock 
		{
			while(deque_send.full())
				pthread_cond_wait(...);
			deque_send.set(msg);
			pthread_cond_signal(deque_send);
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
