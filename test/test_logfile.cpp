
#include "../include/log_file.h"
#include "../src/log_def.h"
#include "unit_test.h"
#include "su_include.h"
#include <thread>

using namespace std;
using namespace su;

namespace
{
	bool f()
	{
		L_COND_F(1 == 1);
		L_COND_F(1 == 2, "dd%d %s", 11, "a");
		L_COND_R(1 == 2, false, "dd%d %s", 11, "a");
		return false;
	}
	void t1()
    {
		f();
        L_INFO("t");
		L_DEBUG("t%d", 1);
		L_WARN("t");
		L_ERROR("ok!. this line print error is ok. t%02d%s", 1, "s");
		L_FATAL("ok!. this line print error is ok. t%02d%s", 1, "s");

		L_COND(1 == 2, "dd%d %s", 11, "a");
    }
	//测试删除文件，会自动创建
	void delLogfile()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			L_DEBUG("t%d", 1);

		}
	}
}//end namespace

UNITTEST(testLogFile)
{
	UNUSED(delLogfile);
	UNUSED(t1);
}