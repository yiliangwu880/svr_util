/*
时间相关
*/

#include "time/alarm_clock.h"
#include <assert.h>
#include <string>
#include "misc.h"
#include "time/sys_time.h"
#include <utility>
#include <time.h>
#include "easy_code.h"
#include "../log_def.h"

using namespace std;
namespace su
{
    using namespace std;

	bool TimeCallBack::CreateTimer(Timer *pTimer, uint32 interval, bool is_loop)
	{
		if (nullptr == pTimer)
		{
			return false;
		}
		if (m_time2data.size() >= MAX_TIMER_NUMBER)
		{
			L_FATAL("timer num is over load!!");
			return 0;
		}

		time_t sec = g_time.Sec();
		time_t key = sec + interval;
		if (key < sec)//时间太大溢出，好几千年都不过期，没意义。
		{
			L_ERROR("timer interval is over load!!");
			return 0;
		}

		inner::CtrlData d;
		d.start_sec = sec;
		d.interval = interval;
		d.is_loop = is_loop;
		d.pTimer = pTimer;

		m_time2data.insert(make_pair(key, d));
		return true;

	}



	bool TimeCallBack::DelTimer(Timer *pTimer)
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

	void TimeCallBack::checkTimeOut()
	{
		time_t sec = g_time.Sec();
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
        FOR_IT(vec_timeout)
		{
			const inner::CtrlData &d = *it;
            if (it->is_loop)
            {
                it->start_sec += it->interval;
                m_time2data.insert(make_pair(it->start_sec+it->interval, *it));
            }
            else
			{
				d.pTimer->StopTimer();

            }
        }

        //保证最后调用，防止回调的时候，调用接口改变对象状态。
		FOR_IT_CONST(vec_timeout)
        {
			const inner::CtrlData &d = *it;
			d.pTimer->OnTimerCB();
        }
	}

	void TimeCallBack::clear()
	{
		FOR_IT_CONST(m_time2data)
		{
			const inner::CtrlData &d = it->second;
			d.pTimer->StopTimer();
		}
		m_time2data.clear();
	}

    uint32 TimeCallBack::GetTimeNum()
    {
        return m_time2data.size();
    }
	
	Timer::Timer()
		: m_state(S_WAIT_START_TIMER)
		, m_cb(nullptr)
	{
	}

	Timer::~Timer()
	{
		StopTimer();//里面保证Timer指针删掉，不会野掉
	}

	void Timer::OnTimerCB()
	{
		m_cb();
	}

	bool Timer::StartTimer(uint32 interval, void *para, bool is_loop)
	{
		auto f = std::bind(&Timer::OnTimer, this, para);
		return StartTimer(interval, f, is_loop);
	}

	bool Timer::StartTimer(uint32 interval, const TimerCB &cb, bool is_loop)
	{
		if (S_WAIT_START_TIMER != m_state)
		{
			//L_ERROR("state error, repeated start timer");
			return false;
		}
		bool ret = TimeCallBack::obj().CreateTimer(this, interval, is_loop);
		if (!ret)
		{
			//L_ERROR("AttachTimer fail");
			return false;
		}
		m_cb = cb;
		m_state = S_WAIT_TIME_OUT;
		return true;
	}

	bool Timer::StopTimer()
	{
		if (S_WAIT_TIME_OUT != m_state)
		{
			return false;
		}
		TimeCallBack::obj().DelTimer(this);//里面保证Timer指针删掉，不会野掉

		m_state = S_WAIT_START_TIMER;
		return true;
	}
}