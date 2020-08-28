/*
brief: 静态注册使用例子
*/

#include "static_trick/publish_subscribe.h"
#include "unit_test.h"
#include "misc.h"
#include <iostream>
#include <map>
#include <string>
#include "stl_boost.h"
#include "static_trick/static_reg.h"

using namespace std;
using namespace su;
namespace
{
	struct Player;
}
namespace su{
//定义你的事件ID对应参数类型
template<>
struct EVENT_ID_INFO<51> {
	using Fun = void(*)(Player &player);
};
template<>
struct EVENT_ID_INFO<52> {
	using Fun = void(*)(Player &player, int i);
};
}
namespace
{
	struct Player;
	struct SubMode1
	{
		int m_f = 0;
		static void f(Player &player);
	};


	struct SubMode2
	{
		int m_f = 0;
		int m_f2 = 0;
		static void f(Player &player);

		static void f2(Player &player, int i);

	};

	struct Player
	{
		PostEvent m_event;
		SubMode1 m_m1;
		SubMode2 m_m2;
		void f();

		//PostTriggerEvent 这里比较遗憾，还找不到通用方法，需要定义所有模板特例
		template<const int ID>
		void PostTriggerEvent()
		{
			auto f = [&]() {
				TriggerEvent<ID>(*this);
			};
			m_event.Add(f);
		}
		template<const int ID, class T1>
		void PostTriggerEvent(const T1 &t1)
		{
			auto f = [&]() {
				TriggerEvent<ID>(*this, t1);
			};
			m_event.Add(f);
		}

		template<const int ID, class ... Args>
		void Event(Args&& ... args)
		{
			::TriggerEvent<ID>(*this, std::forward<Args>(args)...);
		}


		


	};

	void SubMode1::f(Player &player)
	{
		player.m_m1.m_f++;
	}
	void SubMode2::f(Player &player)
	{

		player.m_m2.m_f++;
	}

	void SubMode2::f2(Player &player, int i)
	{
		player.m_m2.m_f2++;
	}

	void Player::f()
	{
		Event< 51>();
		UNIT_ASSERT(m_m1.m_f == 1);
		UNIT_ASSERT(m_m2.m_f == 1);
		Event<52>(3);
		UNIT_ASSERT(m_m2.m_f2 == 1);

		PostTriggerEvent<51>();
		PostTriggerEvent<52>(3);

		UNIT_ASSERT(m_m2.m_f2 == 1);
		m_event.Do();
		UNIT_ASSERT(m_m1.m_f == 2);
		UNIT_ASSERT(m_m2.m_f == 2);
		UNIT_ASSERT(m_m2.m_f2 == 2);

	}
	STATIC_RUN(RegEvent<51>(SubMode1::f););
	STATIC_RUN(RegEvent<51>(SubMode2::f););
	STATIC_RUN(RegEvent<52>(SubMode2::f2););
	struct Init
	{
		Init()
		{
			//RegEvent<51>(SubMode1::f);
			//RegEvent<51>(SubMode2::f);
			//RegEvent<52>(SubMode2::f2);
		}
	} init;
}


UNITTEST(sub_sys_event)
{

	Player player;
	player.f();

}