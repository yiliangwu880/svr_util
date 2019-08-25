

#include "su_include.h"
#include "time/su_timestamp.h"
#include "time/su_timer.h"
#include   <stdio.h>      
#include   <stdio.h>      
#include   <time.h>   
#include "easy_code.h"
#include "unit_test.h"

using namespace std;
using namespace su;
using namespace su;
namespace
{
	//测试SysTime偏移
	void test1()
	{
		TimeDriver::Obj().Clear();
		//这代码测试出错，如果低概率相差一秒就正常。
		SysTime::Obj().SetTimeOffset(0);
		SysTime::Obj().Refresh();
		time_t a = SysTime::Obj().Sec();
		SysTime::Obj().SetTimeOffset(1);
		UNIT_ASSERT(a + 1 == SysTime::Obj().Sec());
		SysTime::Obj().SetTimeOffset(100);
		UNIT_ASSERT(a + 100 == SysTime::Obj().Sec());
		SysTime::Obj().SetTimeOffset(1);
		UNIT_ASSERT(a + 1 == SysTime::Obj().Sec());
		SysTime::Obj().SetTimeOffset(0);
		UNIT_ASSERT(a == SysTime::Obj().Sec());

	}

	static uint32 s_cnt_1 = 0;
	void TimeCBFun2(uint32 para1, void *para2)
	{
		UNIT_ASSERT(1 == para1);
		UNIT_ASSERT((void *)2 == para2);
		s_cnt_1++;
	}
	void test2()
	{
		TimeDriver::Obj().Clear();
		SysTime::Obj().SetTimeOffset(0);
		// g_timer_cb.setTimer(1, TimeCBFun2, 1, (void*)2);
		{
			static Timer tm;
			auto f = std::bind(TimeCBFun2, 1, (void*)2);
			tm.StartTimer(1, f);
		}
		UNIT_ASSERT(1 == TimeDriver::Obj().GetTimeNum());
		SysTime::Obj().SetTimeOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());
		UNIT_ASSERT(1 == s_cnt_1);
		//UNIT_ASSERT(!g_timer_cb.delTimer(0));

	}

	//测试回调里面用timer接口
	static uint32 s_cnt_3 = 0;
	//   static uint32 s_t3_id = 0;
	void TimeCBFun3(uint32 para1, void *para2)
	{
		static Timer tm3;
		UNIT_ASSERT(1 == para1);
		UNIT_ASSERT((void *)0 == para2);
		s_cnt_3++;
		// UNIT_ASSERT(0!=s_t3_id);
		// UNIT_ASSERT(!g_timer_cb.delTimer(s_t3_id)); 
		UNIT_ASSERT(!tm3.StopTimer());
		if (1 == s_cnt_3)
		{
			bool r = false;
			{
				static Timer tm;
				tm.StartTimer(2, std::bind(TimeCBFun3, 1, (void *)0));
			}
			{
				static Timer tm;
				tm.StartTimer(2, std::bind(TimeCBFun3, 1, (void *)0));
			}
			//  s_t3_id = g_timer_cb.setTimer(2, TimeCBFun3, 1);
			 // g_timer_cb.setTimer(2, TimeCBFun3, 1);

			{
				r = tm3.StartTimer(3, std::bind(TimeCBFun3, 1, (void *)0));
				UNIT_ASSERT(r);
			}
			{
				static Timer tm;
				tm.StartTimer(4, std::bind(TimeCBFun3, 1, (void *)0));
			}
			// g_timer_cb.setTimer(3, TimeCBFun3, 1);
			// g_timer_cb.setTimer(4, TimeCBFun3, 1);
			SysTime::Obj().AddTimerOffset(4);
			// UNIT_ASSERT(0!=s_t3_id);
		}
		else if (5 == s_cnt_3)
		{
			{
				static Timer tm;
				tm.StartTimer(9, std::bind(TimeCBFun3, 1, (void *)0));
			}
			//  s_t3_id = g_timer_cb.setTimer(9, TimeCBFun3, 1);
			SysTime::Obj().AddTimerOffset(9);
			// UNIT_ASSERT(0!=s_t3_id);
		}
	}
	void test3()
	{
		SysTime::Obj().SetTimeOffset(0);
		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun3, 1, (void *)0));
		}
		//  s_t3_id = g_timer_cb.setTimer(1, TimeCBFun3, 1);
		 // UNIT_ASSERT(0!=s_t3_id);
		SysTime::Obj().AddTimerOffset(1);
		UNIT_ASSERT(1 == TimeDriver::Obj().GetTimeNum());
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(4 == TimeDriver::Obj().GetTimeNum());
		TimeDriver::Obj().CheckTimeOut();
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(6 == s_cnt_3);
		UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());
	}

	//loop timer回调里面能删除timer
	static uint32 s_cnt_5 = 0;
	void TimeCBFun5(uint32 para1, void *para2)
	{
		s_cnt_5++;
	}
	void test5()
	{
		TimeDriver::Obj().Clear();
		SysTime::Obj().SetTimeOffset(0);

		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun5, 1, (void *)0), true);

			// uint32 id = g_timer_cb.setLoopTimer(1, TimeCBFun5);
			// UNIT_ASSERT(0!=id);
			UNIT_ASSERT(1 == TimeDriver::Obj().GetTimeNum());
			//  UNIT_ASSERT(!g_timer_cb.delTimer(0));
			 // UNIT_ASSERT(g_timer_cb.delTimer(id));
			tm.StopTimer();
		}
		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());
		UNIT_ASSERT(0 == s_cnt_5);

		SysTime::Obj().SetTimeOffset(0);
		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun5, 1, (void *)0), true);

			//  id = g_timer_cb.setLoopTimer(1, TimeCBFun5);
			SysTime::Obj().AddTimerOffset(1);
			TimeDriver::Obj().CheckTimeOut();
			SysTime::Obj().AddTimerOffset(1);
			TimeDriver::Obj().CheckTimeOut();
			SysTime::Obj().AddTimerOffset(2);
			TimeDriver::Obj().CheckTimeOut();
			SysTime::Obj().AddTimerOffset(5);
			for (uint32 i = 0; i < 100; ++i)
			{
				TimeDriver::Obj().CheckTimeOut();
			}
			UNIT_ASSERT(1 == TimeDriver::Obj().GetTimeNum());
			UNIT_ASSERT(9 == s_cnt_5);


			tm.StopTimer(); //UNIT_ASSERT(g_timer_cb.delTimer(id));
			SysTime::Obj().AddTimerOffset(9);
			TimeDriver::Obj().CheckTimeOut();

			UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());
			UNIT_ASSERT(9 == s_cnt_5);
		}
	}


	//测试clear()
	static uint32 s_cnt_6 = 0;
	void TimeCBFun6(uint32 para1, void *para2)
	{
		s_cnt_6++;
	}
	void test6()
	{
		SysTime::Obj().SetTimeOffset(0);
		// g_timer_cb.setTimer(1, TimeCBFun6);
		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun6, 1, (void *)0));
		}
		//g_timer_cb.setLoopTimer(1, TimeCBFun6); 
		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun6, 1, (void *)0), true);
		}
		UNIT_ASSERT(2 == TimeDriver::Obj().GetTimeNum());
		TimeDriver::Obj().Clear();
		UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());
		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		SysTime::Obj().AddTimerOffset(2);
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(0 == s_cnt_6);

		// g_timer_cb.setTimer(1, TimeCBFun6);
		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun6, 1, (void *)0));
		}
		// g_timer_cb.setLoopTimer(1, TimeCBFun6);
		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun6, 1, (void *)0), true);
		}
		SysTime::Obj().AddTimerOffset(7);
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(2 == s_cnt_6);
	}

	//测试timer_id回收  //测试回调调用接口 setTimer //测试回调调用接口 setLoopTimer
	static uint32 s_cnt_7 = 0;
	void TimeCBFun7(uint32 para1, void *para2)
	{
		s_cnt_7++;
		uint32 id = 0;
		bool r = false;
		if (1 == para1)
		{
			//id = g_timer_cb.setTimer(1, TimeCBFun7); 
			{
				static Timer tm;
				r = tm.StartTimer(1, std::bind(TimeCBFun7, 0, (void *)0));
				UNIT_ASSERT(r);
			}
			// UNIT_ASSERT(1 == id);
			 //id = g_timer_cb.setTimer(1, TimeCBFun7, 2);
			{
				static Timer tm;
				r = tm.StartTimer(1, std::bind(TimeCBFun7, 2, (void *)0));
				UNIT_ASSERT(r);
			}
			// UNIT_ASSERT(2 == id);
		}
		else if (2 == para1)
		{
			//id = g_timer_cb.setLoopTimer(1, TimeCBFun7);
			{
				static Timer tm;
				tm.StartTimer(1, std::bind(TimeCBFun7, 1, (void *)0), true);
			}
			//  UNIT_ASSERT(2 == id);
			  //id = g_timer_cb.setTimer(1, TimeCBFun7, 3);
			{
				static Timer tm;
				tm.StartTimer(1, std::bind(TimeCBFun7, 3, (void *)0));
			}
			//  UNIT_ASSERT(1 == id);
		}
		else if (3 == para1)
		{
			//id = g_timer_cb.setTimer(1, TimeCBFun7);
			{
				static Timer tm;
				tm.StartTimer(1, std::bind(TimeCBFun7, 1, (void *)0));
			}
			//UNIT_ASSERT(1 == id);

		}
		if (id)
		{
		}
	}
	void test7()
	{
		bool r = false;
		SysTime::Obj().SetTimeOffset(0);
		TimeDriver::Obj().Clear();
		//g_timer_cb.setTimer(1, TimeCBFun7, 1);
		{
			static Timer tm;
			r = tm.StartTimer(1, std::bind(TimeCBFun7, 1, (void *)0));
			UNIT_ASSERT(r);
		}
		//UNIT_INFO("id=%d", id);
	   // UNIT_ASSERT(1 == id);

		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(5 == s_cnt_7);
	}

	//测试回调调用接口 delTimer
	namespace
	{
		vector< Timer> all_tm_t8(5);
	}
	static uint32 s_cnt_8 = 0;

	void TimeCBFun8(uint32 para1, void *para2)
	{
		s_cnt_8++;
		if (1 == para1)
		{
			//  UNIT_ASSERT(0!=s_8_del_id2);
			//  UNIT_ASSERT(g_timer_cb.delTimer(s_8_del_id2));
			UNIT_ASSERT(all_tm_t8[2].StopTimer());
			//    UNIT_ASSERT(0!=s_8_del_id1);
				//UNIT_ASSERT(!g_timer_cb.delTimer(s_8_del_id1));
			bool r = all_tm_t8[1].StopTimer();
			UNIT_ASSERT(!r);
		}
	}
	void test8()
	{
		SysTime::Obj().SetTimeOffset(0);
		TimeDriver::Obj().Clear();
		// s_8_del_id1 = g_timer_cb.setTimer(1, TimeCBFun8, 1);
		{
			all_tm_t8[1].StartTimer(1, std::bind(TimeCBFun8, 1, (void *)0));
		}
		//s_8_del_id2 = g_timer_cb.setLoopTimer(1, TimeCBFun8, 2);
		{
			static Timer tm;
			all_tm_t8[2].StartTimer(1, std::bind(TimeCBFun8, 2, (void *)0), true);
		}
		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();

		UNIT_ASSERT(2 == s_cnt_8);
	}

	//测试回调调用接口 clear();
	static uint32 s_cnt_9 = 0;
	void TimeCBFun9(uint32 para1, void *para2)
	{
		s_cnt_9++;
		if (1 == para1)
		{
			TimeDriver::Obj().Clear();
		}
	}
	void test9()
	{
		SysTime::Obj().SetTimeOffset(0);
		TimeDriver::Obj().Clear();
		//s_8_del_id1 = g_timer_cb.setLoopTimer(1, TimeCBFun9, 1);
		{
			static Timer tm;
			tm.StartTimer(1, std::bind(TimeCBFun9, 1, (void *)0), true);
		}
		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		SysTime::Obj().AddTimerOffset(2);
		TimeDriver::Obj().CheckTimeOut();

		UNIT_ASSERT(1 == s_cnt_9);
		UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());
	}

	//测试数量溢出
	static uint32 s_cnt_10 = 0;
	void TimeCBFun10(uint32 para1, void *para2)
	{
		s_cnt_10++;
	}
	void test10()
	{
		SysTime::Obj().SetTimeOffset(0);
		TimeDriver::Obj().Clear();
		UNIT_INFO("next line print [fatal] and [error] is ok.");
		//uint32 id_1st = 0;
		vector< Timer> all_tm(NUM_1W);
		for (uint32 i = 0; i < NUM_1W; ++i)
		{
			//uint32 id = g_timer_cb.setLoopTimer(1, TimeCBFun10);
			bool ret = false;
			ret = all_tm[i].StartTimer(1, std::bind(TimeCBFun10, 1, (void *)0), true);
			if (!ret)
			{
				break;
			}
		}
		UNIT_ASSERT(1000 == TimeDriver::Obj().GetTimeNum());
		//g_timer_cb.delTimer(id_1st);
		all_tm[0].StopTimer();
		UNIT_ASSERT(999 == TimeDriver::Obj().GetTimeNum());
		SysTime::Obj().AddTimerOffset(1);
		TimeDriver::Obj().CheckTimeOut();
		UNIT_ASSERT(999 == s_cnt_10);

	}
	namespace
	{
		uint32 stop_cnt = 0;

		class MyTimer : public Timer
		{
		public:
			//timeout会回调函数. timeout回调前变S_WAIT_START_TIMER 状态
			virtual void OnTimer(void *para) 
			{
				stop_cnt++;
				UNIT_ASSERT((void *)11 == para);
			}; 
		};
	}
	void testStop()
	{
		SysTime::Obj().SetTimeOffset(0);
		TimeDriver::Obj().Clear();
		//bool r = false;

		{
			MyTimer t1, t2;
			t1.StartTimer(1, (void *)11);
			t2.StartTimer(1);
			UNIT_ASSERT(t1.StopTimer());
			UNIT_ASSERT(t2.StopTimer());
			t1.StartTimer(1, (void *)11);

			UNIT_ASSERT(!t2.StopTimer());

			UNIT_ASSERT(1 == TimeDriver::Obj().GetTimeNum());
			SysTime::Obj().AddTimerOffset(1);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(1 == stop_cnt);
		}
	}

	namespace
	{
		void testFreeCb()
		{

		}
		void testFreeNoCb()
		{
			UNIT_ASSERT(false);
		}
	}
	void testFree()
	{
		SysTime::Obj().SetTimeOffset(0);
		TimeDriver::Obj().Clear();
		bool r = false;
		{//测试TimeDriver::Obj().clear()
			Timer t1, t2;
			t1.StartTimer(1, nullptr);
			t2.StartTimer(1, testFreeCb);

			r = t1.StartTimer(1, nullptr);
			UNIT_ASSERT(!r);
			r = t2.StartTimer(1, nullptr);
			UNIT_ASSERT(!r);

			UNIT_ASSERT(2 == TimeDriver::Obj().GetTimeNum());
			TimeDriver::Obj().Clear();
			UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());

			r = t1.StartTimer(1, nullptr);
			UNIT_ASSERT(r);
			r = t2.StartTimer(1, nullptr);
			UNIT_ASSERT(r);

			UNIT_ASSERT(2 == TimeDriver::Obj().GetTimeNum());
		}
		//测试局部timer自动释放
		UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());//free t1, t2
		{
			Timer *t1=new Timer();

			UNIT_ASSERT(t1->StartTimer(1, testFreeNoCb));
			UNIT_ASSERT(1 == TimeDriver::Obj().GetTimeNum());
			delete t1; t1 = nullptr;
			SysTime::Obj().AddTimerOffset(1);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(0 == TimeDriver::Obj().GetTimeNum());
		}
		{//illegal arg

			Timer t1;

			UNIT_ASSERT(!t1.StartTimer(0, testFreeNoCb));
		}

	}

	int IncTimer_cnt1=0;
	bool IncTimerCb_Ret = false;
	bool DefIncTimerCb()
	{
		IncTimer_cnt1++;
		return false;
	}

	bool IncTimerCbRet()
	{
		IncTimer_cnt1++;
		return IncTimerCb_Ret;
	}
	void testIncTimer()
	{
		{//test illegal para
			IncTimer timer;
			SysTime::Obj().SetTimeOffset(0);
			TimeDriver::Obj().Clear();
			IncTimer_cnt1 = 0;
			IncTimerCb_Ret = false;


			VecUint32 vi;
			UNIT_ASSERT(!timer.Start(IncTimerCbRet, vi));

			vi = { 0,5,60,80 };
			UNIT_ASSERT(!timer.Start(IncTimerCbRet, vi));


			vi = { 1,0,60,80 };
			UNIT_ASSERT(!timer.Start(IncTimerCbRet, vi));

			vi = { 1,10,60,0 };
			UNIT_ASSERT(!timer.Start(IncTimerCbRet, vi));

		}
		{//test start stop
			IncTimer timer;
			SysTime::Obj().SetTimeOffset(0);
			TimeDriver::Obj().Clear();
			IncTimer_cnt1 = 0;
			IncTimerCb_Ret = false;

			UNIT_ASSERT(timer.Start(DefIncTimerCb));
			UNIT_ASSERT(!timer.Start(DefIncTimerCb));
			UNIT_ASSERT(timer.Stop());
			UNIT_ASSERT(!timer.Stop());


			VecUint32 vi = { 2,5,60,80 };
			timer.Start(IncTimerCbRet, vi);


			SysTime::Obj().AddTimerOffset(vi[0]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(1 == IncTimer_cnt1);


			SysTime::Obj().AddTimerOffset(vi[1]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(2 == IncTimer_cnt1);

			UNIT_ASSERT(timer.Stop());
			SysTime::Obj().AddTimerOffset(vi[2]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(2 == IncTimer_cnt1);

		}
		//测试回调返回true结束
		{
			IncTimer timer;
			SysTime::Obj().SetTimeOffset(0);
			TimeDriver::Obj().Clear();
			IncTimer_cnt1 = 0;
			IncTimerCb_Ret = false;

			VecUint32 vi = { 2,5,60,80 };
			timer.Start(IncTimerCbRet, vi);

			IncTimerCb_Ret = true;
			SysTime::Obj().AddTimerOffset(vi[0]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(1 == IncTimer_cnt1);

			//timer停了
			SysTime::Obj().AddTimerOffset(vi[1]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(1 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(vi[2]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(1 == IncTimer_cnt1);

			UNIT_ASSERT(!timer.Stop());
			UNIT_ASSERT(timer.Start(DefIncTimerCb));

		}
		{//test default Start
			IncTimer timer;
			SysTime::Obj().SetTimeOffset(0);
			TimeDriver::Obj().Clear();
			IncTimer_cnt1 = 0;

			timer.Start(DefIncTimerCb); //10,60,60*5,60*30
			SysTime::Obj().AddTimerOffset(1);
			SysTime::Obj().AddTimerOffset(2);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(0 == IncTimer_cnt1);

			SysTime::Obj().SetTimeOffset(0);
			SysTime::Obj().AddTimerOffset(10);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(1 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(60);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(2 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(60*5);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(3 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(60 * 30);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(4 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(60 * 30);
			TimeDriver::Obj().CheckTimeOut();
			SysTime::Obj().AddTimerOffset(30);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(5 == IncTimer_cnt1);


		}

		{//测试用户指定时间间隔
			IncTimer timer;
			SysTime::Obj().SetTimeOffset(0);
			TimeDriver::Obj().Clear();
			IncTimer_cnt1 = 0;

			VecUint32 vi = { 2,5,60 * 4,60 *5 };
			timer.Start(DefIncTimerCb, vi); 

			SysTime::Obj().AddTimerOffset(1);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(0 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(vi[0]-1);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(1 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(vi[1]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(2 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(vi[2]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(3 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(vi[3]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(4 == IncTimer_cnt1);

			SysTime::Obj().AddTimerOffset(vi[3]);
			TimeDriver::Obj().CheckTimeOut();
			SysTime::Obj().AddTimerOffset(1);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(5 == IncTimer_cnt1);


			//test stop 
			UNIT_ASSERT(timer.Stop());
			UNIT_ASSERT(!timer.Stop());
			SysTime::Obj().AddTimerOffset(vi[3]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(5 == IncTimer_cnt1);
			SysTime::Obj().AddTimerOffset(vi[3]);
			TimeDriver::Obj().CheckTimeOut();
			UNIT_ASSERT(5 == IncTimer_cnt1);

		}
	}
}//end namespace


UNITTEST(timer)
{
	string s;
	SysTime::Obj().TimeToTimeStr(1472688000, s);
	SysTime::Obj().TimeToTimeStr(1470467234, s);
	// time=    1472688000   cur=   1470467234
    test1();
    test2();
    test3();
    //test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
	testFree();
	testStop();
	
}

UNITTEST(IncTimer)
{
	testIncTimer();
}