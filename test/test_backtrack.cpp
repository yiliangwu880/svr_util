/*
brief:支持多线程访问消息队列
*/


#include "BacktraceInfo.h"
#include "unit_test.h"

using namespace std;

namespace
{
  
	void test()
	{
		UNIT_INFO("============\testBacktrace start============\n");
		CBacktraceInfo::Obj().RegHangUpHandler();
		int *p = 0;
		*p = 3;
		UNIT_INFO("============\testBacktrace done================\n");

    }
}//end namespace

UNITTEST(testBacktrace)
{
	//test();
}


