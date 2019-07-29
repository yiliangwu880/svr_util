/*
ʱ�����
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

	bool TimeDriver::NewTimer(Timer *pTimer, uint32 interval, bool is_loop)
	{
		if (0 == interval)
		{
			return false;
		}
		if (nullptr == pTimer)
		{
			return false;
		}
		if (m_time2data.size() >= MAX_TIMER_NUMBER)
		{
			L_FATAL("timer num is over load!!");
			return 0;
		}

		time_t sec = SysTime::Obj().Sec();
		time_t key = sec + interval;
		if (key < sec)//ʱ��̫��������ü�ǧ�궼�����ڣ�û���塣
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

	void TimeDriver::CheckTimeOut()
	{
		SysTime::Obj().Refresh();
		time_t sec = SysTime::Obj().Sec();
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
        //����ѭ����timer
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

        //��֤�����ã���ֹ�ص���ʱ�򣬵��ýӿڸı����״̬��
		FOR_IT_CONST(vec_timeout)
        {
			const inner::CtrlData &d = *it;
			d.pTimer->OnTimerCB();
        }
	}

	void TimeDriver::Clear()
	{
		FOR_IT_CONST(m_time2data)
		{
			const inner::CtrlData &d = it->second;
			d.pTimer->StopTimer();
		}
		m_time2data.clear();
	}

    uint32 TimeDriver::GetTimeNum()
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
		StopTimer();//���汣֤Timerָ��ɾ��������Ұ��
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
		if (0 == interval)
		{
			return false;
		}
		if (S_WAIT_START_TIMER != m_state)
		{
			//L_ERROR("state error, repeated start timer");
			return false;
		}
		bool ret = TimeDriver::Obj().NewTimer(this, interval, is_loop);
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
		TimeDriver::Obj().DelTimer(this);//���汣֤Timerָ��ɾ��������Ұ��

		m_state = S_WAIT_START_TIMER;
		return true;
	}

	bool IncTimer::Start(const IncTimerCB &cb, const VecUint32 &vec_interval)
	{
		if (vec_interval.empty())
		{
			return false;
		}
		FOR_IT_CONST(vec_interval)
		{
			if (*it==0)
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
		//�ɹ��Ÿı����״̬
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
		//����������ʱ��
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
