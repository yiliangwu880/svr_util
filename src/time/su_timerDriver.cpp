
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
namespace su
{
	bool TimeDriver::NewTimer(Timer *pTimer, uint32 interval_sec, bool is_loop)
	{
		if (0 == interval_sec)
		{
			return false;
		}
		if (nullptr == pTimer)
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

		inner::CtrlData d;
		d.start_sec = sec;
		d.interval_sec = interval_sec;
		d.is_loop = is_loop;
		d.pTimer = pTimer;

		m_time2data.insert(make_pair(key, d));
		return true;

	}



	bool TimeDriver::DelTimer(Timer *pTimer)
	{
		if (nullptr == pTimer)
		{
			L_ERROR("error para. nullptr == pTimer");
			return false;
		}
		bool ret = false;
		for (TimeMapData::iterator it = m_time2data.begin(); it != m_time2data.end(); )
		{
			const inner::CtrlData &d = it->second;
			if (d.pTimer == pTimer)
			{
				m_time2data.erase(it++);
				ret = true;
				return true;
			}
			else
			{
				++it;
			}
		}
		return ret;
	}

	TimeDriver::~TimeDriver()
	{
		Clear(); //不掉用，进程关闭的时候就崩溃。因为可能 TimeDriver 先释放，接着timer释放就找不到有效对象了。
	}

	void TimeDriver::CheckTimeOut()
	{
		time_t sec = SysTime::Ins().Sec();
		VecData vec_timeout;

		{
			for (TimeMapData::iterator it = m_time2data.begin(); it != m_time2data.end(); )
			{
				if (it->first > sec)
				{
					break;
				}
				//time out
				const inner::CtrlData &d = it->second;
				vec_timeout.push_back(d);
				m_time2data.erase(it++);
			}
		}
		//重设循环的timer
		for (inner::CtrlData &d : vec_timeout)
		{
			if (d.is_loop)
			{
				d.start_sec += d.interval_sec;
				m_time2data.insert(make_pair(d.start_sec + d.interval_sec, d));
			}
			else
			{
				d.pTimer->StopTimer();

			}
		}

		//保证最后调用，防止回调的时候，调用接口改变对象状态。
		for (const inner::CtrlData &d : vec_timeout)
		{
			d.pTimer->OnTimerCB();
		}
	}

	void TimeDriver::Clear()
	{
		for (auto &v : m_time2data)
		{
			const inner::CtrlData &d = v.second;
			d.pTimer->StopTimer();
		}
		m_time2data.clear();
	}

	uint32 TimeDriver::GetTimeNum()
	{
		return m_time2data.size();
	}
}