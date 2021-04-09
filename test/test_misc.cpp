
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
 

}//end namespace

UNITTEST(testMisc)
{
	test1();
}


namespace su
{


	class B;
	class A
	{
	public:
		int i = 1;
		DoublePoint<A, B> pB;
		A()
			:pB(*this)
		{

		}
		~A()
		{}

	private:

	};

	class B
	{
	public:
		int i = 2;
		DoublePoint<B, A> pA;
		B() 
			:pA(*this)
		{

		}
		~B() {}

	private:

	};
}
UNITTEST(TestPointProxy)
{
	A *a = new A();
	B *b = new B();
	B *b2 = new B();
	b2->i = 3;
	UNIT_ASSERT(nullptr == a->pB);
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!a->pB);
	UNIT_ASSERT(!b->pA);
	a->pB.Attach(b->pA);
	UNIT_ASSERT(nullptr != a->pB);
	UNIT_ASSERT(b->pA != nullptr);
	UNIT_ASSERT(a->pB);
	UNIT_ASSERT(b->pA);

	UNIT_INFO("a->bp->i =%d", a->pB->i);
	UNIT_ASSERT(a->pB->i == 2);
	UNIT_ASSERT(b->pA->i == 1);
	////////////////////////
	a->pB.Detach();
	UNIT_ASSERT(nullptr == a->pB);
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!a->pB);
	UNIT_ASSERT(!b->pA);

	b2->pA.Attach(a->pB);
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!b->pA);

	UNIT_ASSERT(nullptr != a->pB);
	UNIT_ASSERT(b2->pA != nullptr);
	UNIT_ASSERT(a->pB);
	UNIT_ASSERT(b2->pA);
	UNIT_ASSERT(a->pB->i == 3);
	UNIT_ASSERT(b2->pA->i == 1);

	a->pB.Detach();
	UNIT_ASSERT(nullptr == a->pB);
	UNIT_ASSERT(b2->pA == nullptr);
	UNIT_ASSERT(!a->pB);
	UNIT_ASSERT(!b2->pA);
	//////////////测试 自动detach/////////////

	a->pB.Attach(b->pA);
	UNIT_ASSERT(nullptr != a->pB);
	UNIT_ASSERT(b->pA != nullptr);
	UNIT_ASSERT(a->pB);
	UNIT_ASSERT(b->pA);
	UNIT_ASSERT(a->pB->i == 2);
	UNIT_ASSERT(b->pA->i == 1);
	delete a;
	UNIT_ASSERT(b->pA == nullptr);
	UNIT_ASSERT(!b->pA);
	delete b;


	UNIT_INFO("===================end ==========");
}