

#include "su_include.h"
#include "time/su_timestamp.h"
#include "time/su_timer.h"
#include   <stdio.h>      
#include   <stdio.h>      
#include   <time.h>   
#include "easy_code.h"
#include "unit_test.h"
#include "static_trick/publish_subscribe.h"
#include <functional>
#include <type_traits>

using namespace std;
using namespace su;
namespace
{
	void c1()
	{
	}
	void c2(int i)
	{
	}

	void c3(int i,int b)
	{
	}

	void sample()
	{
		RegEvent<1>(c1);
		RegEvent<1>(c2);
		RegEvent<1>(c3);
		TriggerEvent<1>();
		TriggerEvent<1>(1);
		TriggerEvent<1>(1, 2);

	}	
	void test1()
	{
		{
			auto &s1 = GetSubScribeSet<1, decltype(&c1)>();
			auto &s2 = GetSubScribeSet<1, void(*)()>();
			auto ok = std::is_same<decltype(s1), decltype(s2)>::value;
			UNIT_ASSERT(ok);
			UNIT_ASSERT(&s1 == &s2);
		}
		{
			auto &s1 = GetSubScribeSet<1, decltype(&c1)>();
			auto &s2 = GetSubScribeSet<2, decltype(&c1)>();
			auto ok = std::is_same<decltype(s1), decltype(s2)>::value;
			UNIT_ASSERT(ok);
			UNIT_ASSERT(&s1 != &s2);
		}
	}

	int g_cb1 = 0;
	int g_cb11 = 0;
	void cb1()
	{
		g_cb1++;
	}
	void cb11()
	{
		g_cb11++;
	}
	int g_cb2 = 0;
	void cb2(int i)
	{
		g_cb2++;
	}

	int g_cb3 = 0;
	void cb3(int i, int b)
	{
		g_cb3++;
	}
	int g_b_cb3 = 0;
	struct B
	{
		static void cb3(int i, int b)
		{
			g_b_cb3++;
		}
	};
	void test2()
	{
		RegEvent<1>(cb1);
		TriggerEvent<1>();
		UNIT_ASSERT(1 == g_cb1);
		TriggerEvent<2>();
		TriggerEvent<1>(3);
		TriggerEvent<1>(3,4);
		UNIT_ASSERT(1 == g_cb1);

		RegEvent<1>(cb11);
		TriggerEvent<1>();
		UNIT_ASSERT(2 == g_cb1);
		UNIT_ASSERT(1 == g_cb11);

		RegEvent<1>(cb2);
		TriggerEvent<1>(1);
		UNIT_ASSERT(1 == g_cb2);
		RegEvent<1>(cb3);
		TriggerEvent<1>(1, 1);
		UNIT_ASSERT(1 == g_cb3);
		TriggerEvent<1>(1, (int)'1');
		UNIT_ASSERT(2 == g_cb3);
		UNIT_ASSERT(2 == g_cb1);
		RegEvent<1>(B::cb3);
		TriggerEvent<1>(1, 1);
		UNIT_ASSERT(1 == g_b_cb3);
		UNIT_ASSERT(3 == g_cb3);


		enum
		{
			E1=1000,
			E2,
		};

		RegEvent<E1>(cb1);
		TriggerEvent<E1>();
		UNIT_ASSERT(3 == g_cb1);
		RegEvent<E2>(cb1);
		TriggerEvent<E2>();
		UNIT_ASSERT(4 == g_cb1);
		UnRegEvent<E2>(cb1);
		TriggerEvent<E2>();
		UNIT_ASSERT(4 == g_cb1);
		TriggerEvent<E1>();
		UNIT_ASSERT(5 == g_cb1);



		function<void(void)> postEvent = []()
		{
			TriggerEvent<1>(1);
		};
		UNIT_ASSERT(1 == g_cb2);
		postEvent();
		UNIT_ASSERT(2 == g_cb2);



	}

}//end namespace




UNITTEST(publish_subscribe)
{
	sample();
	test1();
	test2();
}