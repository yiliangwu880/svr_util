/* 

*/

#include "su_include.h"
#include "single_progress.h"
#include "unit_test.h"
#include <sys/syscall.h>
#include <unistd.h>

using namespace std;
namespace
{
	int g = 0;
	void SIGUSR1_CB(int sig_type)
	{
		g = 1;
		long long id = syscall(__NR_gettid);
		UNIT_INFO("cb----------------%lld", id);
	}
	void test_signal_follow()
	{
		signal(SIGUSR1, SIGUSR1_CB);

		for (;;)
		{
			g = 0;
			for (int i=0; i<100000; i++)
			{
				int b = i + 34;
				if (b)
				{
				}
				if (1 == g)
				{
					long long id = syscall(__NR_gettid);
					UNIT_INFO("cb change----------------%lld", id);
					g = 0;
				}
			}
		}
	}

}

UNITTEST(test_signal_follow)//中断处理原理，证明中断信号会中断并使用主线程
{
	//test_signal_follow();
	UNUSED(test_signal_follow);
}
