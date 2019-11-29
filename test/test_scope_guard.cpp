

#include "su_include.h"
#include "time/su_timestamp.h"
#include "time/su_timer.h"
#include   <stdio.h>      
#include   <stdio.h>      
#include   <time.h>   
#include "easy_code.h"
#include "unit_test.h"
#include "scope_guard.h"

using namespace std;
using namespace su;
namespace
{
	void fun1(int &i)
	{
		i++;
	}
	void sample()
	{
		int i = 3;
		int b = 3;
		{
			SCOPE_GUARD(
				i = 4;
			i = 5;
			);
			SCOPE_GUARD(b = 4;);
		}
		UNIT_ASSERT(i == 5);
		UNIT_ASSERT(b == 4);
		{
			SCOPE_GUARD(fun1(i));
		}
		UNIT_ASSERT(i == 6);
	}

}//end namespace




UNITTEST(scope_guard)
{
	sample();
}