/*
时间相关
*/

#include "time/su_timer.h"
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
    using namespace std;

	
	Timer::Timer()
		: m_state(S_WAIT_START_TIMER)
		, m_cb(nullptr)
		, m_ctrlData(*this)
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

	bool Timer::StartTimer(uint32 interval_sec, void *para, bool is_loop)
	{
		auto f = std::bind(&Timer::OnTimer, this, para);
		return StartTimer(interval_sec, f, is_loop);
	}

	bool Timer::StartTimer(uint32 interval_sec, const TimerCB &cb, bool is_loop)
	{
		if (0 == interval_sec)
		{
			return false;
		}
		if (S_WAIT_START_TIMER != m_state)
		{
			//L_ERROR("state error, repeated start timer");
			return false;
		}
		bool ret = TimeDriver::Ins().NewTimer(*this, interval_sec, is_loop);
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
		TimeDriver::Ins().DelTimer(*this);//里面保证Timer指针删掉，不会野掉

		m_state = S_WAIT_START_TIMER;
		return true;
	}

	bool IncTimer::Start(const IncTimerCB &cb, const VecUint32 &vec_interval)
	{
		if (vec_interval.empty())
		{
			return false;
		}
		for(auto &v : vec_interval)
		{
			if (v==0)
			{
				return false;
			}
		}
		auto f = std::bind(&IncTimer::TimerCb, this);
		bool ret = m_timer.StartTimer(vec_interval[0], f);
		if (!ret)
		{
			return false;
		}
		//成功才改变对象状态
		m_cb = cb;
		m_vi_idx = 0;
		m_vi = vec_interval;
		return true;
	}

	bool IncTimer::Start(const IncTimerCB &cb)
	{
		VecUint32 vec_interval = { 10,60,60*5,60*30 };
		return Start(cb, vec_interval);
	}

	bool IncTimer::Stop()
	{
		return m_timer.StopTimer();
	}

	void IncTimer::TimerCb()
	{
		bool ret = m_cb();
		if (ret)
		{
			return;
		}
		//继续递增定时器
		m_vi_idx++;
		if (m_vi_idx>(m_vi.size()-1))
		{
			m_vi_idx = m_vi.size() - 1;
		}

		auto f = std::bind(&IncTimer::TimerCb, this);
		ret = m_timer.StartTimer(m_vi[m_vi_idx], f);
		if (!ret)
		{
			L_ERROR("StartTimer fail");
			return;
		}
	}

}
