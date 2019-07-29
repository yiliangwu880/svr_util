
#include "../include/log_file.h"
#include "../src/log_def.h"
#include "unit_test.h"

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

}//end namespace

UNITTEST(testLogFile)
{
		//t1();
}