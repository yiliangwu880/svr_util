/**
author: yilaing.wu

功能：限制程序进程数量为一。 信号结束进程.
会在程序当前目录创建锁文件，比如 my_name.lock


example:
//不建议用，依赖一个timer循环调用，不直观。 以后删掉
启动进程：例子 ./acc_svr
结束进程: 命令参数例子： ./acc_svr stop
int main(int argc, char* argv[])
{
	//start or stop proccess
	SingleProgress::Obj().Check(argc, argv, "acc_svr");
	.... //设置timer一些代码
}
void OnTimer()
{
	if (SingleProgress::Obj().IsExit())
	{
		...//处理结束
		exit(1);
	}
}

*/

#pragma once
#include <string>
#include <signal.h>
#include <functional>
#include "time/su_timer.h"

class file_lock
{
public:
	file_lock(const std::string& file_name);
	~file_lock();

	//para sig, SIGKILL 等值
	int kill(int sig);

	//return true  locked file success;
	bool lock();

	void unlock();
private:
	std::string m_pid_file;
	int m_fd;
};


class SingleProgress
{
	SingleProgress();
public:
	static SingleProgress &Obj()
	{
		static SingleProgress d;
		return d;
	}
	//启动单例进程， 检查main参数， 带"stop"参数就是停止进程。
	//@pname 程序名称，用来区分同文件夹的不同名程序
	void Check(int argc, char* argv[], const char *pname, bool isDaemon = false);

	//根据文件名，检查确保程序唯一进程，多次启动就会结束后启动进程
	void Check(const std::string &single_file_name);
	//请求结束唯一进程。
	void Stop(const std::string &single_file_name);
	//return true表示进程是退出状态。 由用户代码执行退出操作。
	//注意：
	//建议再timer里面不断检查这个状态，根据状态实现退出进程。
	//为什么要建议Timer定时检查if(SingleProgress::Obj().IsExit()) ？
	//因为信号中断函数会中断执行的主线程，中断函数里面修改非局部变量，容易变量冲突，BUG难查。
	bool IsExit(){ return m_is_exit; };

private:
	static void catch_signal(int sig_type);

private:
	bool m_is_exit;//true表示已经收到退出信号，进入退出状态
	sighandler_t m_old_cb;
};



