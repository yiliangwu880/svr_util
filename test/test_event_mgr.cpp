/*
*/

#include "game_util/event_mgr.h"
#include "unit_test.h"
#include "misc.h"
#include <iostream>
#include <map>
#include <string>
#include "stl_boost.h"
#include "static_reg.h"
#include "../src/log_def.h"

using namespace std;
using namespace su;


namespace su
{
	template<>
	struct EventMgrTrait<1> {
		using Fun = void(int i);
	};
	template<>
	struct EventMgrTrait<2> {
		using Fun = void(int a, int b);
	};
	template<>
	struct EventMgrTrait<3> {
		using Fun = void(int a, int b);
	};

}
namespace
{
	struct B1
	{
		virtual ~B1()
		{}
		virtual void v1() {};
	};
	struct B2
	{
		int i;
	};  
	class Player;
	struct Sub1 : public EventCom<Sub1>, B1 , B2
	{
		bool m_r1 = false;
		int m_p1 = 0;
		Player &m_owner;
		Sub1(Player &owner);

		~Sub1() {}
		void OnE1(int i)
		{
			m_r1 = true;
			m_p1 = i;
		}
	};
	struct Sub2 : public EventCom<Sub2>, B1
	{
		bool m_r0 = false;
		bool m_r1 = false;
		bool m_r2 = false;
		int m_p1 = 0;
		int m_p2 = 0;
		Player &m_owner;
		Sub2(Player &owner);
		 ~Sub2() {}
		void clearState()
		{
			m_r0 = false;
			m_r1 = false;
			m_r2 = false;
			m_p1 = 0;
			m_p2 = 0;
		}
		void OnE0()
		{
			m_r0 = true;
		}
		void OnE1(int i)
		{
			m_r1 = true;
			m_p1 = i;

		}
		void OnE2(int a, int b)
		{
			m_r2 = true;
			m_p1 = a;
			m_p2 = b;
		}
		void OnE21(int a, int b)
		{
		}
		void OnE22(int a, int b)
		{
		}
		void OnE23(int a, int b)
		{
		}
		void reg();
	};

	class Player :  public EventMgr, B1
	{
	public:
		Sub1 m_Sub1;
		Sub2 m_Sub2;
		Sub2 *pSub2;
	public:
		Player()
			:m_Sub1(*this)
			, m_Sub2(*this)
		{
			pSub2 = new Sub2(*this);
		}
		~Player() {}

		void f()
		{

		}
		template<int ID, class ... Args>
		void FireEvent2(Args&& ... args)
		{

		}
	};

	struct EventMgrTraits2 {
		using Fun = void();
	};

	void Sub2::reg()
	{
		Reg<0>(&Sub2::OnE0); 
		Reg<1>(&Sub2::OnE1);
		L_INFO("below line print error is ok");
		Reg<1>(&Sub2::OnE1);
		Reg<2>(&Sub2::OnE2);
	}
	Sub1::Sub1(Player &owner)
		:EventCom<Sub1>(owner)
		, m_owner(owner)
	{
		Reg<1>(&Sub1::OnE1);
	}

	Sub2::Sub2(Player &owner)
		: EventCom<Sub2>(owner)
		, m_owner(owner)
	{}


	void test()
	{
		Player m;
		m.m_Sub2.reg();

		m.FireEvent<0>();
		UNIT_ASSERT(!m.m_Sub1.m_r1);
		UNIT_ASSERT(m.m_Sub1.m_p1 == 0);
		UNIT_ASSERT(m.m_Sub2.m_r0);
		m.FireEvent<1>(5);
		UNIT_ASSERT(m.m_Sub1.m_r1);
		UNIT_ASSERT(m.m_Sub1.m_p1 == 5);

		UNIT_ASSERT(m.m_Sub2.m_r1);
		UNIT_ASSERT(m.m_Sub2.m_p1 == 5);

		UNIT_ASSERT(!m.m_Sub2.m_r2);
		m.FireEvent<2>(55, 66);
		UNIT_ASSERT(m.m_Sub2.m_r2);
		UNIT_ASSERT(m.m_Sub2.m_p1 == 55);
		UNIT_ASSERT(m.m_Sub2.m_p2 == 66);

		//////////un reg///
		m.m_Sub2.Unreg<1>(&Sub2::OnE2);//invalid unreg
		m.FireEvent<1>(6);
		UNIT_ASSERT(m.m_Sub2.m_p1 == 6);

		m.m_Sub2.Unreg<1>(&Sub2::OnE1);
		m.FireEvent<1>(7);
		UNIT_ASSERT(m.m_Sub2.m_p1 == 6);


		m.FireEvent<2>(11, 22);
		UNIT_ASSERT(m.m_Sub2.m_p1 == 11);
		UNIT_ASSERT(m.m_Sub2.m_p2 == 22);

		m.m_Sub2.Unreg<2>(&Sub2::OnE2);

		m.FireEvent<2>(1, 2);
		UNIT_ASSERT(m.m_Sub2.m_p1 == 11);
		UNIT_ASSERT(m.m_Sub2.m_p2 == 22);
	}

	void testDelSubCom()
	{
		Player m;
		Sub2 *p = m.pSub2;
		p->Reg<0>(&Sub2::OnE0);
		p->Reg<1>(&Sub2::OnE1);
		p->Reg<2>(&Sub2::OnE2);

		UNIT_ASSERT(!p->m_r0);
		m.FireEvent<0>();
		UNIT_ASSERT(p->m_r0);
		m.FireEvent<1>(5);
		UNIT_ASSERT(p->m_p1 == 5);
		m.FireEvent<2>(1, 2);
		UNIT_ASSERT(p->m_p1 == 1);
		UNIT_ASSERT(p->m_p2 == 2);

		p->clearState();
		UNIT_ASSERT(p->m_p2 == 0);

		UNIT_ASSERT(m.GetObserverNum<0>() == 1);
		UNIT_ASSERT(m.GetObserverNum<1>() == 2);
		UNIT_ASSERT(m.GetObserverNum<2>() == 1);
		delete p;
		UNIT_ASSERT(m.GetObserverNum<0>() == 0);
		UNIT_ASSERT(m.GetObserverNum<1>() == 1);
		UNIT_ASSERT(m.GetObserverNum<2>() == 0);

		m.FireEvent<0>();
		m.FireEvent<1>(5556);
		UNIT_ASSERT(m.m_Sub1.m_p1 == 5556);
		m.FireEvent<2>(1, 2);

		Player * pp = new Player;
		delete pp;

	

	}
	void testDelSubCom2()
	{
		Player m;
		Sub2 *p = m.pSub2;
		p->Reg<0>(&Sub2::OnE0);
		p->Reg<1>(&Sub2::OnE1);
		p->Reg<2>(&Sub2::OnE2);
		p->Reg<2>(&Sub2::OnE21);
		p->Reg<2>(&Sub2::OnE22);
		p->Reg<2>(&Sub2::OnE23);

		UNIT_ASSERT(m.GetObserverNum<0>() == 1);
		UNIT_ASSERT(m.GetObserverNum<1>() == 2);
		UNIT_ASSERT(m.GetObserverNum<2>() == 4);
		delete p;
		UNIT_ASSERT(m.GetObserverNum<0>() == 0);
		UNIT_ASSERT(m.GetObserverNum<1>() == 1);
		UNIT_ASSERT(m.GetObserverNum<2>() == 0);
	}
}


UNITTEST(event_mgr)
{

	test();
	testDelSubCom();
	testDelSubCom2();

}
