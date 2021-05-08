
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
		L_COND(1 == 2, false, "dd%d %s", 11, "a");
		L_COND(1 == 2, false);
		
		COND(1 == 2, false);
		COND_F(1 == 1);
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

		L_COND_V(1 == 2, "dd%d %s", 11, "a");

		COND_V(1 == 2);
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
	void TestEffectSize()
	{
		time_t start = time(nullptr);
		for (int i = 0; i < 1000 ; i++)
		{
			string str;
			str.assign(1024 * 10, 'a');
			L_DEBUG("t%s", str.c_str());
		}
		time_t end = time(nullptr);
		L_DEBUG("%ld", end - start);
	}
}//end namespace

UNITTEST(testLogFile)
{
	//TestEffectSize();
//	f();
//	delLogfile();
	//L_ASSERT(1 == 4);
}