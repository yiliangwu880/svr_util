

#include "string_tool.h"
#include <cassert>
#include "unit_test.h"
#include "misc.h"
#include <set>
#include <map>
#include "easy_code.h"

using namespace std;
using namespace su;

namespace
{
	void test1()
	{
		//test create id
		{
			IdCreater c;
			set<uint64> s;
			bool ret = s.insert(c.CreateId()).second;
			UNIT_ASSERT(ret);
			ret = s.insert(c.CreateId()).second;
			UNIT_ASSERT(ret);
			for (uint32 i=0; i<1000; ++i)
			{
				ret = s.insert(c.CreateId()).second;
				UNIT_ASSERT(ret);
			}
		}

		{
			IdCreater c1, c2;
			bool ret = false;
			set<uint64> s;
			ret = s.insert(c1.CreateId()).second;
			UNIT_ASSERT(ret);
			ret = s.insert(c2.CreateId()).second;
			UNIT_ASSERT(ret==false); //非常低概率，再秒时间边界运行会失败.
		}
		//test FOR_IT
		{
			vector<int> vec = { 1};
			FOR_IT(vec)
			{
				UNIT_ASSERT(*it == 1);
			}
			const vector<int> &c_vec = vec;
			FOR_IT(c_vec)
			{
				UNIT_ASSERT(*it == 1);
			}
			map<int, int> m = { 
				make_pair(1,11)
				//,make_pair(2,22)
			};
			FOR_IT(m)
			{
				UNIT_ASSERT(it->first == 1);
				UNIT_ASSERT(it->second == 11);
			}
			const map<int, int> &cm = m;
			FOR_IT(cm)
			{
				UNIT_ASSERT(it->first == 1);
				UNIT_ASSERT(it->second == 11);
			}
		}
	}
 

}//end namespace

UNITTEST(testMisc)
{
	test1();



}