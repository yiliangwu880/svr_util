


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
 

    void TestSysTime()
    {
        UNIT_ASSERT(0 == SysTime::GetMonthPeriodCnt(0, 0));
        UNIT_ASSERT(1 == SysTime::GetMonthPeriodCnt(0, 1));
        UNIT_ASSERT(13 == SysTime::GetMonthPeriodCnt(1, 1));

        UNIT_ASSERT(SysTime::IsLeapYear(1972));
        UNIT_ASSERT(!SysTime::IsLeapYear(1970));
        UNIT_ASSERT(SysTime::IsLeapYear(2000));
        UNIT_ASSERT(!SysTime::IsLeapYear(2100));

        UNIT_ASSERT(29==SysTime::GetDaysInMonth(1972, 2));
        UNIT_ASSERT(28==SysTime::GetDaysInMonth(1970, 2));
        UNIT_ASSERT(29==SysTime::GetDaysInMonth(2000, 2));
        UNIT_ASSERT(28==SysTime::GetDaysInMonth(2100, 2));

        g_time.SetTimeOffset(0);
        g_time.Refresh();
        time_t real_sec = g_time.Sec();
        UNIT_ASSERT(0 != real_sec);

        //测试调试时间
        ///////////////////////////////////
        g_time.AddTimerOffset(0);
        const char *p1 = NULL;

        p1 = "2015-01-01 00-00-01";
        UNIT_ASSERT(g_time.SetTimeByStr(p1));
        UNIT_ASSERT(p1 == g_time.GetTimeStr());

        p1 = "1970-01-01 07-00-01";
        UNIT_ASSERT(!g_time.SetTimeByStr(p1));
        UNIT_ASSERT("2015-01-01 00-00-01" == g_time.GetTimeStr());
     
        p1 = "1970-01-01 08-00-01";
        UNIT_ASSERT(g_time.SetTimeByStr(p1));
        UNIT_ASSERT(p1 == g_time.GetTimeStr());
        g_time.AddTimerOffset(3600);
        UNIT_ASSERT("1970-01-01 09-00-01" == g_time.GetTimeStr());
        
        ///////////////////////////////////
        
        UNIT_ASSERT(g_time.SetTimeByStr("1972-02-01 08-00-01"));
        UNIT_ASSERT(g_time.IsLeapYear());
        UNIT_ASSERT(29==g_time.GetDaysInMonth());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-02-01 08-00-00"));
        UNIT_ASSERT(!g_time.IsLeapYear());
        UNIT_ASSERT(28==g_time.GetDaysInMonth());
        UNIT_ASSERT(g_time.SetTimeByStr("2000-02-01 08-00-01"));
        UNIT_ASSERT(g_time.IsLeapYear());
        UNIT_ASSERT(29==g_time.GetDaysInMonth());
        UNIT_ASSERT(g_time.SetTimeByStr("2100-02-01 08-00-01"));
        UNIT_ASSERT(!g_time.IsLeapYear());
		UNIT_ASSERT(28==g_time.GetDaysInMonth());
		UNIT_ASSERT(g_time.SetTimeByStr("2010-02-01 08-00-01"));
		UNIT_ASSERT(!g_time.IsLeapYear());

        UNIT_ASSERT(g_time.SetTimeByStr("2100-01-01 00-00-00"));
        UNIT_ASSERT(31==g_time.GetDaysInMonth());
        UNIT_ASSERT(g_time.SetTimeByStr("2100-08-01 00-00-00"));
        UNIT_ASSERT(31==g_time.GetDaysInMonth());
        UNIT_ASSERT(g_time.SetTimeByStr("2100-09-01 00-00-00"));
        UNIT_ASSERT(30==g_time.GetDaysInMonth());
        UNIT_ASSERT(g_time.SetTimeByStr("2100-12-41 00-00-00"));//故意写41日，里面会调整
        UNIT_ASSERT(31==g_time.GetDaysInMonth());

        //测试getMonthPeriodCnt
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-01 08-00-00"));
        UNIT_ASSERT(1 == g_time.GetMonthPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-31 23-59-59"));;
        UNIT_ASSERT(1 == g_time.GetMonthPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-02-22 00-00-00"));
        UNIT_ASSERT(2 == g_time.GetMonthPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("2015-12-22 00-00-00"));
        UNIT_ASSERT(552 == g_time.GetMonthPeriodCnt());

        //测试getDayPeriodCnt
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-01 08-00-00"));
        UNIT_ASSERT(0 == g_time.GetDayPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-04 00-00-00"));
        UNIT_ASSERT(0 == g_time.GetDayPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-04 23-59-59"));
        UNIT_ASSERT(0 == g_time.GetDayPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-05 00-00-00"));
        UNIT_ASSERT(1 == g_time.GetDayPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-06 00-00-00"));
        UNIT_ASSERT(2 == g_time.GetDayPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("2015-01-06 00-00-00"));
        UNIT_ASSERT(16438 == g_time.GetDayPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("2015-01-05 23-59-59"));
        UNIT_ASSERT(16437 == g_time.GetDayPeriodCnt());

        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-04 00-00-00"));
        UNIT_ASSERT(0 == g_time.GetWeekPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-11 00-00-00"));
        UNIT_ASSERT(1 == g_time.GetWeekPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-17 23-59-59"));
        UNIT_ASSERT(1 == g_time.GetWeekPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("1970-01-18 00-00-00"));
        UNIT_ASSERT(2 == g_time.GetWeekPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("2015-12-20 00-00-00"));
        UNIT_ASSERT(2398 == g_time.GetWeekPeriodCnt());
        UNIT_ASSERT(g_time.SetTimeByStr("2015-12-19 23-59-59"));
        UNIT_ASSERT(2397 == g_time.GetWeekPeriodCnt());

    }

    void test11()
    {
        {
            g_time.SetTimeOffset(0);
            uint32 start_mon =  g_time.GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                g_time.AddMonthOffset(1);
            }
            uint32 end_mon = g_time.GetMonthPeriodCnt();
            UNIT_ASSERT(30==end_mon-start_mon);

		}
		{
			g_time.SetTimeOffset(0);
			uint32 start_mon =  g_time.GetMonthPeriodCnt();
			g_time.AddMonthOffset(32);
			uint32 end_mon = g_time.GetMonthPeriodCnt();
			UNIT_ASSERT(32==end_mon-start_mon);
		}
        {
            g_time.SetTimeOffset(0);
            uint32 start_mon =  g_time.GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                g_time.AddMonthOffset(2);
            }
            uint32 end_mon = g_time.GetMonthPeriodCnt();
            UNIT_ASSERT(30*2==end_mon-start_mon);
        }
        {
            g_time.SetTimeOffset(0);
            uint32 start =  g_time.CurTm().tm_year;
            for (uint32 i=0; i<30; ++i)
            {
                g_time.AddYearOffset(2);
            }
            uint32 end = g_time.CurTm().tm_year;
            UNIT_ASSERT(30*2==end-start);
        }
    }

	void test12()
	{
		g_time.SetTimeOffset(0);
		g_time.Refresh();
		time_t real_sec = g_time.Sec();
		UNIT_ASSERT(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		g_time.AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "2016-05-05 00-00-01";
		UNIT_ASSERT(g_time.SetTimeByStr(p1));
		UNIT_ASSERT(p1 == g_time.GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-01-01 00-00-00");
		}

		p1 = "2015-05-05 00-00-00";
		UNIT_ASSERT(g_time.SetTimeByStr(p1));
		UNIT_ASSERT(p1 == g_time.GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-03 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(g_time.Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-01-01 00-00-00");
		}
	}
	void test13()
	{
		g_time.SetTimeOffset(0);
		g_time.Refresh();
		time_t real_sec = g_time.Sec();
		UNIT_ASSERT(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		g_time.AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "a";
		UNIT_ASSERT(!g_time.SetTimeByStr(p1));
		p1 = "2015-01-";
		UNIT_ASSERT(!g_time.SetTimeByStr(p1));
		p1 = "2015-01-01";
		UNIT_ASSERT(g_time.SetTimeByStr(p1));
		UNIT_ASSERT("2015-01-01 00-00-00" == g_time.GetTimeStr());
	}

}//end namespace

UNITTEST(test_wyl_time)
{
	string s;
	g_time.TimeToTimeStr(1472688000, s);
	g_time.TimeToTimeStr(1470467234, s);
	// time=    1472688000   cur=   1470467234
    TestSysTime();
   
	test11();
	test12();
	test13();

	
}