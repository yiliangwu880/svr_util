
#include "time/su_timer.h"
#include "time/su_timerDriver.h"
#include <assert.h>
#include <string>
#include "misc.h"
#include "time/su_timestamp.h"
#include <utility>
#include <time.h>
#include "easy_code.h"
#include "../log_def.h"

using namespace std;

using namespace su::inner;
namespace su
{
	bool TimeDriver::NewTimer(Timer &timer, uint32 interval_sec, bool is_loop)
	{
		if (0 == interval_sec)
		{
			return false;
		}


		time_t sec = SysTime::Ins().Sec();
		time_t key = sec + interval_sec;
		if (key < sec)//时间太大溢出，好几千年都不过期，没意义。
		{
			L_ERROR("timer interval is over load!!");
			return 0;
		}

		inner::CtrlData &d = timer.m_ctrlData;
		d.end_sec = key;
		d.start_sec = sec;
		d.interval_sec = interval_sec;
		d.is_loop = is_loop;

		m_all.Push(d);
		return true;

	}



	bool TimeDriver::DelTimer(Timer &timer)
	{
		bool ret = false;
		return m_all.Erase(timer.m_ctrlData);
	}

	TimeDriver::~TimeDriver()
	{
		Clear(); //不掉用，进程关闭的时候就崩溃。因为可能 TimeDriver 先释放，接着timer释放就找不到有效对象了。
	}

	void TimeDriver::CheckTimeOut()
	{
		time_t sec = SysTime::Ins().Sec();
		std::vector<Timer *> vec;
		while (true)
		{
			CtrlData *p= m_all.Front();
			if (nullptr == p)
			{
				break;
			}
			if (p->end_sec > sec)
			{
				break;
			}
			Timer* pTimer = &(p->m_owner);
			//time out
			m_all.Pop();
			vec.push_back(pTimer);
		}

		for (Timer* pTimer : vec)
		{
			//重设循环的timer
			CtrlData &d = pTimer->m_ctrlData;
			if (d.is_loop)
			{
				d.start_sec += d.interval_sec;
				d.end_sec = d.start_sec + d.interval_sec;
				m_all.Push(d);
			}
			else
			{
				pTimer->StopTimer();
			}
		}
		for (Timer* pTimer: vec)
		{
			//保证最后调用，防止回调的时候，调用接口改变对象状态。
			pTimer->OnTimerCB();
		}
	}

	void TimeDriver::Clear()
	{
		std::vector<CtrlData *> vec;
		auto f=[&](CtrlData *p) {
			vec.push_back(p);
		};
		m_all.Foreach(f);
		for (CtrlData *p: vec)
		{
			p->m_owner.StopTimer();
		}
		m_all.clear();
	}

	uint32 TimeDriver::GetTimeNum()
	{
		return m_all.size();
	}
}