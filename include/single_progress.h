/**
author: yilaing.wu

���ܣ����Ƴ����������Ϊһ�� �źŽ�������.
���ڳ���ǰĿ¼�������ļ������� my_name.lock


example:
�������̣�
void main()
{
	SingleProgress::Obj().CheckSingle("my_name");
	.... //����timerһЩ����
}
void OnTimer()
{
	if (SingleProgress::Obj().IsExit())
	{
		...//�������
		exit(1);
	}
}

�������̣� ͨ��������һ�����̣�ͣ��ָ�����̣�
void main()
{
	SingleProgress::Obj().StopSingle("my_name");

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

	//para sig, SIGKILL ��ֵ
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

	//�����ļ��������ȷ������Ψһ���̣���������ͻ��������������
	void CheckSingle(const std::string &single_file_name);
	//�������Ψһ���̡�
	void StopSingle(const std::string &single_file_name);
	//return true��ʾ�������˳�״̬�� ���û�����ִ���˳�������
	//������timer���治�ϼ�����״̬������״̬ʵ���˳����̡�
	//ΪʲôҪ����Timer��ʱ���if(SingleProgress::Obj().IsExit()) ��
	//��Ϊ�ź��жϺ������ж�ִ�е����̣߳��жϺ��������޸ķǾֲ����������ױ�����ͻ��BUG�Ѳ顣
	bool IsExit(){ return m_is_exit; };

private:
	static void catch_signal(int sig_type);

private:
	bool m_is_exit;
	sighandler_t m_old_cb;
};


typedef std::function<void(void)> ExitProccessCB;
//���߲�Ӧ�� SingleProgress
class SPCheckArg
{
	SPCheckArg()
		:m_cb(nullptr)
	{}
public:
	static SPCheckArg &Obj()
	{
		static SPCheckArg d;
		return d;
	}
	//@fun ��������
	//@para const char *pname, ������
	//@para ExitProccessCB c, ����رս���ǰ����Ҫ�����߼�������ص�����ִ�С�
	void Start(const char *pname, ExitProccessCB cb);
	//@fun �رս���
	//@para const char *pname, ������
	void Stop(const char *pname);
private:
	void CheckStopProccess();

private:
	ExitProccessCB m_cb; 
	su::Timer m_tm;
};

