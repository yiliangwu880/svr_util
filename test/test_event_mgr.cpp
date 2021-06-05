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
using namespace std::placeholders;




namespace su
{
	//template<>
	//struct EventMgrTraits<0> {
	//	using Fun = void();
	//};

	template<>
	struct EventMgrTraits<1> {
		using Fun = void(int i);
	};

	template<>
	struct EventMgrTraits<2> {
		using Fun = void(int a, int b);
	};
	template<>
	struct EventMgrTraits<3> {
		using Fun = void(int a, int b);
	};

	


}
namespace
{

	class Player;


	struct Sub1 : public EventCom<Sub1>
	{
		Player &m_owner;
		Sub1(Player &owner);

	};


	struct Sub2 : public EventCom<Sub2>
	{
		Player &m_owner;
		Sub2(Player &owner);
		void OnE0()
		{
			L_INFO("OnE0");
		}
		void OnE1(int i)
		{
			L_INFO("OnE1 %d",  i);

		}
		void OnE2(int a, int b)
		{
			L_INFO("OnE2 %d %d", a, b);

		}
		void reg();
	};

	class Player :  public EventMgr
	{
	public:
		Sub1 m_Sub1;
		Sub2 m_Sub2;
	public:
		Player()
			:m_Sub1(*this)
			, m_Sub2(*this)
		{
	
		}

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
	{}

	Sub2::Sub2(Player &owner)
		: EventCom<Sub2>(owner)
		, m_owner(owner)
	{}
}




UNITTEST(event_mgr)
{
	Player m;
	m.m_Sub2.reg();
	m.FireEvent<0>();
	m.FireEvent<1>(5);
	m.FireEvent<2>(5,6);
	L_INFO("end");


}