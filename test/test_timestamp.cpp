

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

        SysTime::Obj().SetTimeOffset(0);
        SysTime::Obj().Refresh();
        time_t real_sec = SysTime::Obj().Sec();
        UNIT_ASSERT(0 != real_sec);

        //测试调试时间
        ///////////////////////////////////
        SysTime::Obj().AddTimerOffset(0);
        const char *p1 = NULL;

        p1 = "2060-01-01 00-00-01";
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr(p1));
        UNIT_ASSERT(p1 == SysTime::Obj().GetTimeStr());

        p1 = "1970-01-01 07-00-01";
        UNIT_ASSERT(!SysTime::Obj().SetTimeByStr(p1));
        UNIT_ASSERT("2060-01-01 00-00-01" == SysTime::Obj().GetTimeStr());
     
		SysTime::Obj().ClearOffsetForTest();
        p1 = "1970-01-01 08-00-01";
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr(p1, true));
        UNIT_ASSERT(p1 == SysTime::Obj().GetTimeStr());
        SysTime::Obj().AddTimerOffset(3600);
        UNIT_ASSERT("1970-01-01 09-00-01" == SysTime::Obj().GetTimeStr());
        
        ///////////////////////////////////
        
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1972-02-01 08-00-01", true));
        UNIT_ASSERT(SysTime::Obj().IsLeapYear());
        UNIT_ASSERT(29==SysTime::Obj().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-02-01 08-00-00", true));
        UNIT_ASSERT(!SysTime::Obj().IsLeapYear());
        UNIT_ASSERT(28==SysTime::Obj().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2000-02-01 08-00-01", true));
        UNIT_ASSERT(SysTime::Obj().IsLeapYear());
        UNIT_ASSERT(29==SysTime::Obj().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2100-02-01 08-00-01", true));
        UNIT_ASSERT(!SysTime::Obj().IsLeapYear());
		UNIT_ASSERT(28==SysTime::Obj().GetDaysInMonth());
		UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2010-02-01 08-00-01", true));
		UNIT_ASSERT(!SysTime::Obj().IsLeapYear());

        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2100-01-01 00-00-00", true));
        UNIT_ASSERT(31==SysTime::Obj().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2100-08-01 00-00-00", true));
        UNIT_ASSERT(31==SysTime::Obj().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2100-09-01 00-00-00", true));
        UNIT_ASSERT(30==SysTime::Obj().GetDaysInMonth());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2100-12-41 00-00-00", true));//故意写41日，里面会调整
        UNIT_ASSERT(31==SysTime::Obj().GetDaysInMonth());

        //测试getMonthPeriodCnt
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-01 08-00-00", true));
        UNIT_ASSERT(1 == SysTime::Obj().GetMonthPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-31 23-59-59", true));;
        UNIT_ASSERT(1 == SysTime::Obj().GetMonthPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-02-22 00-00-00", true));
        UNIT_ASSERT(2 == SysTime::Obj().GetMonthPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2015-12-22 00-00-00", true));
        UNIT_ASSERT(552 == SysTime::Obj().GetMonthPeriodCnt());

        //测试getDayPeriodCnt
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-01 08-00-00", true));
        UNIT_ASSERT(0 == SysTime::Obj().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-04 00-00-00", true));
        UNIT_ASSERT(0 == SysTime::Obj().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-04 23-59-59", true));
        UNIT_ASSERT(0 == SysTime::Obj().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-05 00-00-00", true));
        UNIT_ASSERT(1 == SysTime::Obj().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-06 00-00-00", true));
        UNIT_ASSERT(2 == SysTime::Obj().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2015-01-06 00-00-00", true));
        UNIT_ASSERT(16438 == SysTime::Obj().GetDayPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2015-01-05 23-59-59", true));
        UNIT_ASSERT(16437 == SysTime::Obj().GetDayPeriodCnt());

        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-04 00-00-00", true));
        UNIT_ASSERT(0 == SysTime::Obj().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-11 00-00-00", true));
        UNIT_ASSERT(1 == SysTime::Obj().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-17 23-59-59", true));
        UNIT_ASSERT(1 == SysTime::Obj().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("1970-01-18 00-00-00", true));
        UNIT_ASSERT(2 == SysTime::Obj().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2015-12-20 00-00-00", true));
        UNIT_ASSERT(2398 == SysTime::Obj().GetWeekPeriodCnt());
        UNIT_ASSERT(SysTime::Obj().SetTimeByStr("2015-12-19 23-59-59", true));
        UNIT_ASSERT(2397 == SysTime::Obj().GetWeekPeriodCnt());

    }

    void test11()
    {
        {
            SysTime::Obj().SetTimeOffset(0);
            uint32 start_mon =  SysTime::Obj().GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                SysTime::Obj().AddMonthOffset(1);
            }
            uint32 end_mon = SysTime::Obj().GetMonthPeriodCnt();
            UNIT_ASSERT(30==end_mon-start_mon);

		}
		{
			SysTime::Obj().SetTimeOffset(0);
			uint32 start_mon =  SysTime::Obj().GetMonthPeriodCnt();
			SysTime::Obj().AddMonthOffset(32);
			uint32 end_mon = SysTime::Obj().GetMonthPeriodCnt();
			UNIT_ASSERT(32==end_mon-start_mon);
		}
        {
            SysTime::Obj().SetTimeOffset(0);
            uint32 start_mon =  SysTime::Obj().GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                SysTime::Obj().AddMonthOffset(2);
            }
            uint32 end_mon = SysTime::Obj().GetMonthPeriodCnt();
            UNIT_ASSERT(30*2==end_mon-start_mon);
        }
        {
            SysTime::Obj().SetTimeOffset(0);
            uint32 start =  SysTime::Obj().CurTm().tm_year;
            for (uint32 i=0; i<30; ++i)
            {
                SysTime::Obj().AddYearOffset(2);
            }
            uint32 end = SysTime::Obj().CurTm().tm_year;
            UNIT_ASSERT(30*2==end-start);
        }
    }

	void test12()
	{
		SysTime::Obj().SetTimeOffset(0);
		SysTime::Obj().Refresh();
		time_t real_sec = SysTime::Obj().Sec();
		UNIT_ASSERT(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		SysTime::Obj().AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "2016-05-05 00-00-01";
		UNIT_ASSERT(SysTime::Obj().SetTimeByStr(p1, true));
		UNIT_ASSERT(p1 == SysTime::Obj().GetTimeStr() || "2016-05-05 01-00-01" == SysTime::Obj().GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2016-01-01 00-00-00");
		}

		p1 = "2015-05-05 00-00-00";
		UNIT_ASSERT(SysTime::Obj().SetTimeByStr(p1, true));
		UNIT_ASSERT(p1 == SysTime::Obj().GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-03 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(SysTime::Obj().Sec());
			UNIT_ASSERT(SysTime::TimeToTimeStr(sec, str));
			UNIT_ASSERT(str=="2015-01-01 00-00-00");
		}
	}
	void test13()
	{
		SysTime::Obj().SetTimeOffset(0);
		SysTime::Obj().Refresh();
		time_t real_sec = SysTime::Obj().Sec();
		UNIT_ASSERT(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		SysTime::Obj().AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "a";
		UNIT_ASSERT(!SysTime::Obj().SetTimeByStr(p1, true));
		p1 = "2015-01-";
		UNIT_ASSERT(!SysTime::Obj().SetTimeByStr(p1, true));
		p1 = "2015-01-01";
		UNIT_ASSERT(SysTime::Obj().SetTimeByStr(p1, true));
		UNIT_ASSERT("2015-01-01 00-00-00" == SysTime::Obj().GetTimeStr());
	}

}//end namespace

UNITTEST(test_wyl_time)
{
	string s;
	SysTime::Obj().TimeToTimeStr(1472688000, s);
	SysTime::Obj().TimeToTimeStr(1470467234, s);
	// time=    1472688000   cur=   1470467234
    TestSysTime();
   
	test11();
	test12();
	test13();

	
}