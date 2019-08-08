

#include "time/su_cd.h"
#include "time/su_timestamp.h"
#include "time/su_timer.h"
#include "su_include.h"

#include   <stdio.h>      
#include   <stdio.h>      
#include   <time.h>   
#include <cstring>
#include "unit_test.h"


//#ifdef   WIN32      
//#define   LOCALTIME_R(t)     localtime((t))      
//#else      
//#define   LOCALTIME_R(t)     localtime_r((t),   (struct   tm   *)&tmres)      
//#endif   

#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace su;
namespace
{

void currentTime()     
{        
	time_t long_time=0;     
	time(&long_time); 
	tm   tmNow;
	localtime_r(&long_time, &tmNow);
	//printf("%d年%d月%d日   %d时%d分%d秒 周%d",tmNow.tm_year,   tmNow.tm_mon   +   1,    tmNow.tm_mday,   tmNow.tm_hour,   tmNow.tm_min,   tmNow.tm_sec
	//	, tmNow.tm_wday
	//	);     

	tm   re;
	memset(&re,0,sizeof(tm));
	re.tm_min = 1;
	re.tm_mday = 1;
	mktime( &re);
	return;     
}  

void testCurCycleNum()
{
	{	
		//1970,1,4， 0时0分0秒 
		CurCycleNum week1(3600*24*7, 3600*24*0);
		tm tmNow ;
		tmNow.tm_sec = 0;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 0; 
		tmNow.tm_mday = 4;		//4 == week 0
		tmNow.tm_mon = 0;	//0 start
		tmNow.tm_year = 1970-1900;
		tmNow.tm_isdst = 0 ;

		time_t ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_mday = 3;	
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_mday = 10;	 //week 6
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_sec = 59;
		tmNow.tm_min = 59; 
		tmNow.tm_hour = 23; 
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_sec = 0;
		tmNow.tm_min = 0;
		tmNow.tm_hour = 0;
		tmNow.tm_mday = 11;	 //week 0
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==1);
		tmNow.tm_mday = 18;	 //week 0
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==2);
		tmNow.tm_mday = 24;	 //week 0
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==2);
	}
	{	
		//1970,1,4， 0时0分0秒 ，周期分界为周一+10秒
		CurCycleNum week1(3600*24*7, 3600*24*1+10);
		tm tmNow ;
		tmNow.tm_sec = 0;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 0; 
		tmNow.tm_mday = 4;		//4 == week 0
		tmNow.tm_mon = 0;	//0 start
		tmNow.tm_year = 1970-1900;
		tmNow.tm_isdst = 0 ;

		time_t ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_mday = 3;	
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_mday = 12;	 //week 1
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_sec = 9;
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==0);

		tmNow.tm_sec = 10;
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==1);

		tmNow.tm_mday = 12;	 //week 1
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==1);
	
		tmNow.tm_mday = 19;	 //week 0
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==2);
	}
	{	
		//2014,7,x，  ，周期分界为周一+10秒
		CurCycleNum week1(3600*24*7, 3600*24*1+10);
		tm tmNow ;
		tmNow.tm_sec = 0;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 0; 
		tmNow.tm_mday = 27;		// week 0
		tmNow.tm_mon = 6;	//7 start
		tmNow.tm_year = 2014-1900;
		tmNow.tm_isdst = 0 ;


		tmNow.tm_mday = 28;		// week 1
		tmNow.tm_sec = 9;
		time_t zero = week1.Get(mktime( &tmNow  ));


		tmNow.tm_mday = 28;		// week 1
		tmNow.tm_sec = 10;
		time_t ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+1);

		tmNow.tm_mday = 29;		// week 2
		 ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+1);
		

		tmNow.tm_mday = 4;		// week 1
		tmNow.tm_mon = 7;	
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+2);

		tmNow.tm_mday = 4;		// week 1
		tmNow.tm_mon = 7;	
		tmNow.tm_sec = 9;
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+1);
	}
	
	{	
		//2014,7,x，  ，周期分界为周一+10秒
		CurCycleNum week1(3600*24,0);
		tm tmNow ;
		tmNow.tm_sec = 0;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 0; 
		tmNow.tm_mday = 27;		// week 0
		tmNow.tm_mon = 6;	//7 start
		tmNow.tm_year = 2014-1900;
		tmNow.tm_isdst = 0 ;


		tmNow.tm_mday = 28;		// week 1
		time_t zero = week1.Get(mktime( &tmNow  ));

		tmNow.tm_mday = 29;	
		time_t ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+1);

		tmNow.tm_mday = 29;	
		tmNow.tm_sec = 59;
		tmNow.tm_min = 59; 
		tmNow.tm_hour = 23; 
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+1);
	}
	{
		CurCycleNum week1(3600*24, 10);
		tm tmNow ;
		tmNow.tm_sec = 0;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 0; 
		tmNow.tm_mday = 27;		// week 0
		tmNow.tm_mon = 6;	//7 start
		tmNow.tm_year = 2014-1900;
		tmNow.tm_isdst = 0 ;


		tmNow.tm_mday = 28;		// week 1
		tmNow.tm_sec = 9;
		time_t zero = week1.Get(mktime( &tmNow  ));

		tmNow.tm_mday = 29;	
		time_t ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+1);

		tmNow.tm_mday = 29;	
		tmNow.tm_sec = 10; 
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+2);
	}
	{
		CurCycleNum week1(3600/2, 10);
		tm tmNow ;
		tmNow.tm_sec = 0;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 0; 
		tmNow.tm_mday = 27;		// week 0
		tmNow.tm_mon = 6;	//7 start
		tmNow.tm_year = 2014-1900;
		tmNow.tm_isdst = 0 ;


		tmNow.tm_sec = 10;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 0; 	
		time_t zero = week1.Get(mktime( &tmNow  ));

		tmNow.tm_sec = 10;
		tmNow.tm_min = 30; 
		tmNow.tm_hour = 0; 
		time_t ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+1);

		tmNow.tm_sec = 9;
		tmNow.tm_min = 30; 
		tmNow.tm_hour = 0; 
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+0);

		tmNow.tm_sec = 10;
		tmNow.tm_min = 0; 
		tmNow.tm_hour = 1; 
		ret = week1.Get(mktime( &tmNow  ));
		UNIT_ASSERT(ret==zero+2);

	}
}

void testPeriodAccumulate()
{
    SysTime::Obj().SetTimeOffset(0);
	PeriodAccumulate obj(1,10); 
	UNIT_ASSERT(10 ==obj.currentNum());
	obj.costNum(10);
	UNIT_ASSERT(0 ==obj.currentNum());
	SysTime::Obj().AddTimerOffset(1);
	UNIT_ASSERT(1 ==obj.currentNum());
	obj.costNum(1);
	UNIT_ASSERT(0 ==obj.currentNum());
	obj.addExtNum(11);
	UNIT_ASSERT(11 ==obj.currentNum());
	SysTime::Obj().AddTimerOffset(1);
	UNIT_ASSERT(11 ==obj.currentNum());
	obj.setMaxNum(12);
	UNIT_ASSERT(11 ==obj.currentNum());
	SysTime::Obj().AddTimerOffset(1);
	UNIT_ASSERT(12 ==obj.currentNum());
	obj.addExtNum(1);
	UNIT_ASSERT(13 ==obj.currentNum());
	
	obj.costNum(100);
	obj.setMaxNum(10);
	UNIT_ASSERT(0 ==obj.currentNum());
	SysTime::Obj().AddTimerOffset(1);


}


void waitUtill1()
{
	UNUSED(waitUtill1);
	for (;;)//wait untill 1sec period over
	{
		time_t t;
		time(&t);
		if (t%2==1)
		{
			//Info(t);
			break;
		}
	}
}
void waitUtill2()
{
	UNUSED(waitUtill2);
	for (;;)//wait untill 2sec period over
	{
		time_t t;
		time(&t);
		if (t%2==0)
		{
		//	Info(t);
			break;
		}
	}
}
void testSysPeriodCD()
{
    SysTime::Obj().SetTimeOffset(0);
	CdDefault obj(2);
	
	UNIT_ASSERT(!obj.IsCD());

	obj.StartCD();
	UNIT_ASSERT(obj.IsCD());
	SysTime::Obj().AddTimerOffset(2);
	UNIT_ASSERT(!obj.IsCD());


	obj.StartCD();
	UNIT_ASSERT(obj.IsCD());

	obj.StartCD();
	UNIT_ASSERT(obj.IsCD());
	obj.ClearCD();
	UNIT_ASSERT(!obj.IsCD());

}

void testAccumulCDBase()
{
	class AccumulCD : public AccumulCDBase<time_t>
	{
	public:
		AccumulCD(const time_t &forceCdDist)
			:AccumulCDBase(forceCdDist)
		{
		}
		virtual time_t CurrentTick() const
		{
			return SysTime::Obj().Sec();
		}
	};
	
    {
        SysTime::Obj().SetTimeOffset(0);
		AccumulCD obj(4);
		UNIT_ASSERT(!obj.IsForceCD());
		UNIT_ASSERT(0 == obj.GetCD());
		obj.AddCD(1);
		UNIT_ASSERT(1 == obj.GetCD());
		obj.AddCD(1);
		UNIT_ASSERT(2 == obj.GetCD());
		obj.AddCD(2);
		UNIT_ASSERT(4 == obj.GetCD());
		UNIT_ASSERT(obj.IsForceCD());

        SysTime::Obj().AddTimerOffset(4);
		UNIT_ASSERT(!obj.IsForceCD());
		UNIT_ASSERT(0 == obj.GetCD());

		obj.AddCD(1);
		UNIT_ASSERT(1 == obj.GetCD());
		obj.ClearCD();
		UNIT_ASSERT(!obj.IsForceCD());
		UNIT_ASSERT(0 == obj.GetCD());
	}
	{
		AccumulCD obj(4);
		UNIT_ASSERT(!obj.IsForceCD());
		UNIT_ASSERT(0 == obj.GetCD());
		obj.AddCD(1);
		UNIT_ASSERT(1 == obj.GetCD());
		obj.AddCD(1);
		UNIT_ASSERT(2 == obj.GetCD());
		obj.AddCD(2);
		UNIT_ASSERT(4 == obj.GetCD());
		UNIT_ASSERT(obj.IsForceCD());
		obj.ClearCD();
		UNIT_ASSERT(!obj.IsForceCD());
		UNIT_ASSERT(0 == obj.GetCD());
	}
}

void testNormalCdBase()
{

	{
        SysTime::Obj().SetTimeOffset(0);
		CdDefault obj(2);

		UNIT_ASSERT(!obj.IsCD());
		obj.StartCD();
		UNIT_ASSERT(obj.IsCD());
        SysTime::Obj().AddTimerOffset(2);
		UNIT_ASSERT(!obj.IsCD());


		obj.StartCD();
		UNIT_ASSERT(obj.IsCD());
		obj.ClearCD();
		UNIT_ASSERT(!obj.IsCD());
	}


    {
        SysTime::Obj().SetTimeOffset(0);
		CdDefault obj(2);

		UNIT_ASSERT(!obj.IsCD());
		obj.StartCD();
		UNIT_ASSERT(obj.GetCountDownTime()>1);
		UNIT_ASSERT(obj.IsCD());
		obj.ChangePeriod(3);
		UNIT_ASSERT(obj.GetCountDownTime()>2);
		SysTime::Obj().AddTimerOffset(2);
		UNIT_ASSERT(obj.IsCD());
		SysTime::Obj().AddTimerOffset(1);
		UNIT_ASSERT(!obj.IsCD());

	}
}

void testNormalCdBase2()
{
	typedef CdDefault SysCD;

    {
        SysTime::Obj().SetTimeOffset(0);
		SysCD obj(2);

		UNIT_ASSERT(!obj.IsCD());
		obj.StartCD();
		UNIT_ASSERT(obj.IsCD());
        SysTime::Obj().AddTimerOffset(2);
		UNIT_ASSERT(!obj.IsCD());


		obj.StartCD();
		UNIT_ASSERT(obj.IsCD());
		obj.ClearCD();
		UNIT_ASSERT(!obj.IsCD());
	}


    {
        SysTime::Obj().SetTimeOffset(0);
		SysCD obj(2);

		UNIT_ASSERT(!obj.IsCD());
		obj.StartCD();
		UNIT_ASSERT(obj.GetCountDownTime()>1);
		UNIT_ASSERT(obj.IsCD());
		obj.ChangePeriod(3);
		UNIT_ASSERT(obj.GetCountDownTime()>2);
        SysTime::Obj().AddTimerOffset(2);
		UNIT_ASSERT(obj.IsCD());
		SysTime::Obj().AddTimerOffset(1);
		UNIT_ASSERT(!obj.IsCD());

	}
}
void testTimertt()
{
    //低概率，在系统1秒分界点跑这个代码就会通不过
	TimerCnt obj;
    SysTime::Obj().SetTimeOffset(0);

	UNIT_ASSERT(0 == obj.peerCurrentTimer());
	obj.start();
	UNIT_ASSERT(0 == obj.peerCurrentTimer());
    SysTime::Obj().SetTimeOffset(1);
    UNIT_ASSERT(1 == obj.peerCurrentTimer());
    SysTime::Obj().SetTimeOffset(2);
    UNIT_ASSERT(2 == obj.peerCurrentTimer());
    SysTime::Obj().SetTimeOffset(5);
	obj.pause();
    UNIT_ASSERT(5 == obj.peerCurrentTimer());
    SysTime::Obj().SetTimeOffset(10);
	UNIT_ASSERT(5 == obj.peerCurrentTimer());
	obj.resume();
    UNIT_ASSERT(5 == obj.peerCurrentTimer());
    SysTime::Obj().SetTimeOffset(11);
	UNIT_ASSERT(6 == obj.peerCurrentTimer());
    SysTime::Obj().SetTimeOffset(12);
	UNIT_ASSERT(7 == obj.end());


}

void testPeriodCnt()
{//很低概率时间会不对，测试出错
	SysTime::Obj().SetTimeOffset(0);
	PeriodCnt obj(2,1);
	PeriodCnt obj2(2,1);
	UNIT_ASSERT(0 == obj.previewCnt());
	UNIT_ASSERT(0 == obj.getCnt());

	SysTime::Obj().AddTimerOffset(2);
	obj2 = obj;
	UNIT_ASSERT(1== obj.previewCnt());
	UNIT_ASSERT(1 == obj.getCnt());
	UNIT_ASSERT(0 == obj.getCnt());
	UNIT_ASSERT(1== obj2.previewCnt());
	UNIT_ASSERT(1 == obj2.getCnt());
	UNIT_ASSERT(0 == obj2.getCnt());
	SysTime::Obj().AddTimerOffset(2);
	UNIT_ASSERT(1 == obj.previewCnt());
	UNIT_ASSERT(1 == obj.getCnt());

}
	void testPeriodResetCnt()
	{
        SysTime::Obj().SetTimeOffset(0);
		//uint8 buf[100];
		//Memory out(buf, 100);
		//const Memory in(buf, 100);



        {//1个重置
            SysTime::Obj().SetTimeOffset(0);
            SysTime::Obj().SetTimeByStr("2016-01-31-0-0-0");//周日0时
       

            VecInt64 vec_limit;
            vec_limit.push_back(3600*24*0);
			CycleMoreReset obj(3600*24*7, vec_limit);

			time_t start = obj.GetCurCycleNum();
  
            SysTime::Obj().AddTimerOffset(3600*24*1); 
            UNIT_ASSERT(start == obj.GetCurCycleNum());
            SysTime::Obj().AddTimerOffset(3600*24*5); 
            UNIT_ASSERT(start == obj.GetCurCycleNum());
            UNIT_ASSERT("2016-02-06 00-00-00" == SysTime::Obj().GetTimeStr());
            UNIT_ASSERT(6 == SysTime::Obj().CurTm().tm_wday);
			SysTime::Obj().AddTimerOffset(3600*24*1); //流逝一日
			UNIT_ASSERT(0 == SysTime::Obj().CurTm().tm_wday);
			UNIT_ASSERT(start+1 == obj.GetCurCycleNum());
			SysTime::Obj().AddTimerOffset(3600*24*7); //流逝一周
			UNIT_ASSERT(0 == SysTime::Obj().CurTm().tm_wday);
			UNIT_ASSERT(start+2 == obj.GetCurCycleNum());
        }
        {//2个重置
            SysTime::Obj().SetTimeOffset(0);
            SysTime::Obj().SetTimeByStr("2016-01-31-0-0-0");//周日0时
            VecInt64 vec_limit;
            vec_limit.push_back(3600*24*0);//周日
            vec_limit.push_back(3600*24*1);//周一
			CycleMoreReset obj(3600*24*7, vec_limit);
			time_t start = obj.GetCurCycleNum();

			SysTime::Obj().AddTimerOffset(3600*24*1); 
			UNIT_ASSERT(start+1 == obj.GetCurCycleNum());
			SysTime::Obj().AddTimerOffset(3600*24*5); 
			UNIT_ASSERT(start+1 == obj.GetCurCycleNum());
            UNIT_ASSERT("2016-02-06 00-00-00" == SysTime::Obj().GetTimeStr());
            UNIT_ASSERT(6 == SysTime::Obj().CurTm().tm_wday);
			SysTime::Obj().AddTimerOffset(3600*24*1); //流逝一周
			UNIT_ASSERT(start+2 == obj.GetCurCycleNum());

			SysTime::Obj().AddTimerOffset(3600*24*1); 
			UNIT_ASSERT(1 == SysTime::Obj().CurTm().tm_wday);//周一
			UNIT_ASSERT(start+3 == obj.GetCurCycleNum());

			SysTime::Obj().AddTimerOffset(3600*24*7); 
			UNIT_ASSERT(1 == SysTime::Obj().CurTm().tm_wday);//周一
			UNIT_ASSERT(start+5 == obj.GetCurCycleNum());

        }
        {//3个重置
            SysTime::Obj().SetTimeOffset(0);
            SysTime::Obj().SetTimeByStr("2016-01-31-0-0-0");//周日0时
            VecInt64 vec_limit;
			vec_limit.push_back(3600*24*3);//周三
			vec_limit.push_back(3600*24*0);//周日
            vec_limit.push_back(3600*24*1);//周一
			CycleMoreReset obj(3600*24*7, vec_limit);
			time_t start = obj.GetCurCycleNum();

			SysTime::Obj().AddTimerOffset(3600*24*1); 
			UNIT_ASSERT(start+1 == obj.GetCurCycleNum());
			SysTime::Obj().AddTimerOffset(3600*24*5); 
			UNIT_ASSERT(start+2 == obj.GetCurCycleNum());
			UNIT_ASSERT("2016-02-06 00-00-00" == SysTime::Obj().GetTimeStr());
			UNIT_ASSERT(6 == SysTime::Obj().CurTm().tm_wday);
			SysTime::Obj().AddTimerOffset(3600*24*1); //流逝一周
			UNIT_ASSERT(start+3 == obj.GetCurCycleNum());

			SysTime::Obj().AddTimerOffset(3600*24*1); 
			UNIT_ASSERT(1 == SysTime::Obj().CurTm().tm_wday);//周一
			UNIT_ASSERT(start+4 == obj.GetCurCycleNum());
			SysTime::Obj().AddTimerOffset(3600*24*1); 
			UNIT_ASSERT(2 == SysTime::Obj().CurTm().tm_wday);//周二
			UNIT_ASSERT(start+4 == obj.GetCurCycleNum());
			SysTime::Obj().AddTimerOffset(3600*24*1); 
			UNIT_ASSERT(3== SysTime::Obj().CurTm().tm_wday);//周三
			UNIT_ASSERT(start+5 == obj.GetCurCycleNum());

			SysTime::Obj().AddTimerOffset(3600*24*7); 
			UNIT_ASSERT(3 == SysTime::Obj().CurTm().tm_wday);//周三
			UNIT_ASSERT(start+8 == obj.GetCurCycleNum());
        }
	}

	void test1()
	{
		SysTime::Obj().Refresh();
		UNIT_ASSERT(0 == CurCycleNum::GetCurPeriodNum(SysTime::Obj().Sec()+1, 2));
		UNIT_ASSERT(0 == CurCycleNum::GetCurPeriodNum(SysTime::Obj().Sec(), 2));
		UNIT_ASSERT(0 == CurCycleNum::GetCurPeriodNum(SysTime::Obj().Sec()-1, 2));
		UNIT_ASSERT(1 == CurCycleNum::GetCurPeriodNum(SysTime::Obj().Sec()-2, 2));
	}
}//end namespace


UNITTEST(test_cd)
{
	test1();
	currentTime();
	testCurCycleNum();
	testSysPeriodCD();
	testTimertt();
	testPeriodResetCnt();

	testPeriodAccumulate();
	testPeriodCnt();
	testAccumulCDBase();
	testNormalCdBase();
	testNormalCdBase2();
}