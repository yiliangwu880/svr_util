

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
			auto &s1 = inner::GetChannel<1, decltype(&c1)>();
			auto &s2 = inner::GetChannel<1, void(*)()>();
			auto ok = std::is_same<decltype(s1), decltype(s2)>::value;
			UNIT_ASSERT(ok);
			UNIT_ASSERT(&s1 == &s2);
		}
		{
			auto &s1 = inner::GetChannel<1, decltype(&c1)>();
			auto &s2 = inner::GetChannel<2, decltype(&c1)>();
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

	bool g_cb12 = false;

	bool g_cb121 = false;
	void recursion_cb2()
	{
		g_cb12 = true;
	}
	void recursion_cb12()
	{
		TriggerEvent<11>();
	}
	void recursion_cb()
	{
		RegEvent<12>(recursion_cb12);
		RegEvent<11>(recursion_cb2);
	}
	void recursion_cb12_1()
	{
		g_cb121 = true;
		TriggerEvent<12>();
	}
	void test3()
	{
		//test recursion trigger

		RegEvent<11>(recursion_cb);
		UNIT_INFO("======next line error is ok=======");
		TriggerEvent<11>();
		TriggerEvent<12>();
		RegEvent<12>(recursion_cb12_1);
		UNIT_INFO("======next line error is ok=======");
		TriggerEvent<12>();

		UNIT_ASSERT(!g_cb12);
		UNIT_ASSERT(g_cb121);
	}


	struct PostPlayer
	{
		bool num = 0;
		void fun()
		{
			num++;
		}
	};
	void PostHandlePlayer(PostPlayer &player)
	{
		player.num++;
	}	
	int g_post_num = 0;
	int g_post_void_num = 0;
	void PostHandle(int i)
	{
		g_post_num = i;
	}
	void PostHandleVoid()
	{
		g_post_void_num ++;
	}

	//延时调用
	void test_post_event()
	{
		//还不能用，参考吧。变量模板有问题
		RegEvent<104>(PostHandle);
		//int i = 11;
		PostTriggerEventExample<104>(1);
		DoPostEvent();

		UNIT_ASSERT(1 == g_post_num);
		//UNIT_ASSERT(1 == player.num);

	}
}//end namespace




UNITTEST(publish_subscribe)
{
	//test_post_event();
	sample();
	test1();
	test2();
	test3();
}

namespace
{
	struct A
	{
		bool is_f = false;
		int f(int i)
		{
			is_f = true;
			return 1;
		}
	};
	int g_event1 = 0;
	void event1()
	{
		g_event1++;
	}


	int g_event2 = 0;
	int event2( int i)
	{
		g_event2 = i;
		return 1;
	}
}

UNITTEST(PostEvent)
{
	A a;
	PostEvent p;
	p.Add(std::bind(&A::f, &a, 1));
	p.Add(std::bind(event1));
	p.Add(event1);
	{
		auto f = []() {event2(33); };
		p.Add(f);
	}
	p.Do();
	UNIT_ASSERT(a.is_f);
	UNIT_ASSERT(g_event1 == 2);
	UNIT_ASSERT(g_event2 == 33);

	p.Add(event1);
	p.Add(std::bind(&A::f, &a, 1));
	p.Do();
	UNIT_ASSERT(g_event1 == 3);


}