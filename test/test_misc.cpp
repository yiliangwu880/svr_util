
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
	template <typename Owner, typename Other>
	class DoublePoint
	{
		friend class DoublePoint<Other, Owner>;
		Other *othter = nullptr;
		DoublePoint<Other, Owner> *otherProxy = nullptr;
		Owner &owner;
	public:
		DoublePoint(Owner &in_owner)
			:owner(in_owner)
		{
		}
		Other * operator->() const
		{
			return othter;
		}
		explicit operator bool() const noexcept
		{
			return othter != nullptr;
		}
	

		void Attach(DoublePoint<Other, Owner> &bp)
		{
			if (nullptr != othter)
			{
				Detach();
			}
			othter = &(bp.owner);
			otherProxy = &bp;

			bp.othter = &owner;
			bp.otherProxy = this;
		}
		void Detach()
		{
			otherProxy->othter = nullptr;
			othter = nullptr;
			//节省点效率，不改变 otherProxy
			//otherProxy->otherProxy = nullptr;
			//otherProxy = nullptr;
		}
	private:
	}; 


	template<typename _Tp, typename _Dp>
	inline bool
		operator==(nullptr_t, const DoublePoint<_Tp, _Dp>& __x) noexcept
	{
		return !__x;
	}
	template<typename _Tp, typename _Dp>
	inline bool
		operator==(const DoublePoint<_Tp, _Dp>& __x, nullptr_t) noexcept
	{
		return !__x;
	}
	template<typename _Tp, typename _Dp>
	inline bool
		operator!=(nullptr_t, const DoublePoint<_Tp, _Dp>& __x) noexcept
	{
		return bool(__x);
	}
	template<typename _Tp, typename _Dp>
	inline bool
		operator!=(const DoublePoint<_Tp, _Dp>& __x, nullptr_t) noexcept
	{
		return bool(__x);
	}

	class B;
	class A
	{
	public:
		int i = 1;
		DoublePoint<A, B> bp;
		A()
			:bp(*this)
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
		DoublePoint<B, A> bp;
		B() 
			:bp(*this)
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
	UNIT_ASSERT(nullptr == a->bp);
	UNIT_ASSERT(b->bp == nullptr);
	UNIT_ASSERT(!a->bp);
	UNIT_ASSERT(!b->bp);
	{
		unique_ptr<A> p;
		if (nullptr == p)
		{
		}	
		if (p == nullptr )
		{
		}
		if (p)
		{
		}
	}
	a->bp.Attach(b->bp);
	UNIT_ASSERT(nullptr != a->bp);
	UNIT_ASSERT(b->bp != nullptr);
	UNIT_ASSERT(a->bp);
	UNIT_ASSERT(b->bp);

	UNIT_INFO("a->bp->i =%d", a->bp->i);
	UNIT_ASSERT(a->bp->i == 2);
	UNIT_ASSERT(b->bp->i == 1);

	UNIT_INFO("===================end ==========");
	UNIT_ASSERT(false);
}