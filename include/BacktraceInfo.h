/**
*@brief ���̱����������ջ��־
* ͨ������
int backtrace (void **buffer, int size);
char **backtrace_symbols (void *const *buffer, int size);
*	��ȡ��ջ��Ϣ

��������ѡ����Ҫ�� -rdynamic

�鿴��־ʱ��������c++filt���߽�����Ϣ�����磺
c++filt _Z18print_stack_framesi

�鿴��ַ���ļ�λ�ã����磺
addr2line -e ./zonesvr 0xbd0f18
*/

#pragma once
#include <string>



	class CBacktraceInfo
	{
		typedef void(*sig_t)(int);
	public:
		static CBacktraceInfo &Obj()
		{
			static CBacktraceInfo d;
			return d;
		}
		CBacktraceInfo();
		/**
		*��ȡ���ж�ջ��Ϣ
		*@return ���ַ��������Ķ�ջ��Ϣ, ���᷵��NULL
		*/
		const char* GetBacktrackInfo();

		//main��������ע�ᣬ�����ͻᵱǰĿ¼�����ջ��־�ļ���hangup_log.log����
		void RegHangUpHandler();

	private:
		void WidebrightSegvHandler(int signum);
		static void s_WidebrightSegvHandler(int signum);

	private:
		const static int MAX_BACKTRACE_SYMBOLS_NUMBER = 50; //!<����ӡ�Ķ�ջ�����
		sig_t s_old_SIGABRT;
		sig_t s_old_SIGSEGV;
		sig_t s_old_SIGFPE;
	};

#define BT_STR()   CBacktraceInfo::GetBacktrackInfo()


