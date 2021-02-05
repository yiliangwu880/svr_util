

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

        SysTime::Ins().SetTimeOffset(0);
        SysTime::Ins().Refresh();
        time_t real_sec = SysTime::Ins().Sec();
        UNIT_ASSERT(0 != real_sec);

        //测试调试时间
        ///////////////////////////////////
        SysTime::Ins().AddTimerOffset(0);
        const char *p1 = NULL;

        p1 = "2060-01-01 00-00-01";
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr(p1));
        UNIT_ASSERT(p1 == SysTime::Ins().GetTimeStr());

        p1 = "1970-01-01 07-00-01";
        UNIT_ASSERT(!SysTime::Ins().SetTimeByStr(p1));
        UNIT_ASSERT("2060-01-01 00-00-01" == SysTime::Ins().GetTimeStr());
     
		SysTime::Ins().ClearOffsetForTest();
        p1 = "1970-01-01 08-00-01";
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr(p1, true));
        UNIT_ASSERT(p1 == SysTime::Ins().GetTimeStr());
        SysTime::Ins().AddTimerOffset(3600);
        UNIT_ASSERT("1970-01-01 09-00-01" == SysTime::Ins().GetTimeStr());
        
        ///////////////////////////////////
        
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1972-02-01 08-00-01", true));
        UNIT_ASSERT(SysTime::Ins().IsLeapYear());
        UNIT_ASSERT(29==SysTime::Ins().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-02-01 08-00-00", true));
        UNIT_ASSERT(!SysTime::Ins().IsLeapYear());
        UNIT_ASSERT(28==SysTime::Ins().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2000-02-01 08-00-01", true));
        UNIT_ASSERT(SysTime::Ins().IsLeapYear());
        UNIT_ASSERT(29==SysTime::Ins().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2100-02-01 08-00-01", true));
        UNIT_ASSERT(!SysTime::Ins().IsLeapYear());
		UNIT_ASSERT(28==SysTime::Ins().GetDaysInMonth());
		UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2010-02-01 08-00-01", true));
		UNIT_ASSERT(!SysTime::Ins().IsLeapYear());

        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2100-01-01 00-00-00", true));
        UNIT_ASSERT(31==SysTime::Ins().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2100-08-01 00-00-00", true));
        UNIT_ASSERT(31==SysTime::Ins().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2100-09-01 00-00-00", true));
        UNIT_ASSERT(30==SysTime::Ins().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2100-12-41 00-00-00", true));//故意写41日，里面会调整
        UNIT_ASSERT(31==SysTime::Ins().GetDaysInMonth());

        //测试getMonthPeriodCnt
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-01 08-00-00", true));
        UNIT_ASSERT(1 == SysTime::Ins().GetMonthPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-31 23-59-59", true));;
        UNIT_ASSERT(1 == SysTime::Ins().GetMonthPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-02-22 00-00-00", true));
        UNIT_ASSERT(2 == SysTime::Ins().GetMonthPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2015-12-22 00-00-00", true));
        UNIT_ASSERT(552 == SysTime::Ins().GetMonthPeriodCnt());

        //测试getDayPeriodCnt
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-01 08-00-00", true));
        UNIT_ASSERT(0 == SysTime::Ins().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-04 00-00-00", true));
        UNIT_ASSERT(0 == SysTime::Ins().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-04 23-59-59", true));
        UNIT_ASSERT(0 == SysTime::Ins().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-05 00-00-00", true));
        UNIT_ASSERT(1 == SysTime::Ins().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-06 00-00-00", true));
        UNIT_ASSERT(2 == SysTime::Ins().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2015-01-06 00-00-00", true));
        UNIT_ASSERT(16438 == SysTime::Ins().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2015-01-05 23-59-59", true));
        UNIT_ASSERT(16437 == SysTime::Ins().GetDayPeriodCnt());

        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-04 00-00-00", true));
        UNIT_ASSERT(0 == SysTime::Ins().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-11 00-00-00", true));
        UNIT_ASSERT(1 == SysTime::Ins().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-17 23-59-59", true));
        UNIT_ASSERT(1 == SysTime::Ins().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("1970-01-18 00-00-00", true));
        UNIT_ASSERT(2 == SysTime::Ins().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2015-12-20 00-00-00", true));
        UNIT_ASSERT(2398 == SysTime::Ins().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Ins().SetTimeByStr("2015-12-19 23-59-59", true));
        UNIT_ASSERT(2397 == SysTime::Ins().GetWeekPeriodCnt());

    }

    void test11()
    {
        {
            SysTime::Ins().SetTimeOffset(0);
            uint32 start_mon =  SysTime::Ins().GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                SysTime::Ins().AddMonthOffset(1);
            }
            uint32 end_mon = SysTime::Ins().GetMonthPeriodCnt();
            UNIT_ASSERT(30==end_mon-start_mon);

		}
		{
			SysTime::Ins().SetTimeOffset(0);
			uint32 start_mon =  SysTime::Ins().GetMonthPeriodCnt();
			SysTime::Ins().AddMonthOffset(32);
			uint32 end_mon = SysTime::Ins().GetMonthPeriodCnt();
			UNIT_ASSERT(32==end_mon-start_mon);
		}
        {
            SysTime::Ins().SetTimeOffset(0);
            uint32 start_mon =  SysTime::Ins().GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                SysTime::Ins().AddMonthOffset(2);
            }
            uint32 end_mon = SysTime::Ins().GetMonthPeriodCnt();
            UNIT_ASSERT(30*2==end_mon-start_mon);
        }
        {
            SysTime::Ins().SetTimeOffset(0);
            uint32 start =  SysTime::Ins().CurTm().tm_year;
            for (uint32 i=0; i<30; ++i)
            {
                SysTime::Ins().AddYearOffset(2);
            }
            uint32 end = SysTime::Ins().CurTm().tm_year;
            UNIT_ASSERT(30*2==end-start);
        }
    }

	void test12()
	{
		SysTime::Ins().SetTimeOffset(0);
		SysTime::Ins().Refresh();
		time_t real_sec = SysTime::Ins().Sec();
		UNIT_ASSERT(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		SysTime::Ins().AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "2016-05-05 00-00-01";
		UNIT_ASSERT(SysTime::Ins().SetTimeByStr(p1, true));
		UNIT_ASSERT(p1 == SysTime::Ins().GetTimeStr() || "2016-05-05 01-00-01" == SysTime::Ins().GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-01-01 00-00-00");
		}

		p1 = "2015-05-05 00-00-00";
		UNIT_ASSERT(SysTime::Ins().SetTimeByStr(p1, true));
		UNIT_ASSERT(p1 == SysTime::Ins().GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-03 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(SysTime::Ins().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-01-01 00-00-00");
		}
	}
	void test13()
	{
		SysTime::Ins().SetTimeOffset(0);
		SysTime::Ins().Refresh();
		time_t real_sec = SysTime::Ins().Sec();
		UNIT_ASSERT(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		SysTime::Ins().AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "a";
		UNIT_ASSERT(!SysTime::Ins().SetTimeByStr(p1, true));
		p1 = "2015-01-";
		UNIT_ASSERT(!SysTime::Ins().SetTimeByStr(p1, true));
		p1 = "2015-01-01";
		UNIT_ASSERT(SysTime::Ins().SetTimeByStr(p1, true));
		UNIT_ASSERT("2015-01-01 00-00-00" == SysTime::Ins().GetTimeStr());
	}

}//end namespace

UNITTEST(test_wyl_time)
{
	string s;
	SysTime::Ins().TimeToTimeStr(1472688000, s);
	SysTime::Ins().TimeToTimeStr(1470467234, s);
	// time=    1472688000   cur=   1470467234
    TestSysTime();
   
	test11();
	test12();
	test13();

	
}