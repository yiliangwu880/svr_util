
#include "su_include.h"
#include "str_util.h"
#include <cassert>
#include "unit_test.h"
#include "misc.h"
#include <set>
#include <map>
#include "easy_code.h"
#include <memory>

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

			uint64 id1 = c1.CreateId();
			uint64 id2 = c1.CreateId();

			//UNIT_INFO("%lld %lld", (id1 & 0xffffffff), (id2 & 0xffffffff));
			ret = ((id1 & 0xffffffff) == (id2 & 0xffffffff));//非常低概率，再秒时间边界运行会失败.
			UNIT_ASSERT(!ret);
			ret = ((id1 >> 32)==(id2 >> 32));
			UNIT_ASSERT(ret);
		}

	}

	struct B
	{

	};
	struct VB
	{
		~VB() {};
		virtual void f() {};
	};
	struct C : public B, public VB, public WeakPtr<C>
	{
		int a;
		int b;
		virtual void fc() {}
	};
	;
	struct CB: public WeakPtr<CB>
	{
		int a;
		int b;

	};

	void testWeakPtr()
	{
		{
			weak_ptr<CB> p;
			{
				CB c;
				p = c;
				shared_ptr<CB> ps = p.lock();
				UNIT_ASSERT(ps.get() == &c);
			}
			shared_ptr<CB> ps = p.lock();
			UNIT_ASSERT(!ps);
		}
		{
			weak_ptr<C> p;
			{
				C c;
				c.a = 1;c.b = 2;
				p = c;
				auto ps = p.lock();
				UNIT_ASSERT(ps);
				UNIT_ASSERT(ps.get() == &c);
				UNIT_ASSERT(ps->a == 1);
				UNIT_ASSERT(ps->b == 2);
			}		
			auto ps = p.lock();
			UNIT_ASSERT(!ps);
		}

	}

}//end namespace

UNITTEST(testMisc)
{
	test1();
	testWeakPtr();
}

