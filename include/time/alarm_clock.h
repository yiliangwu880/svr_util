/*
	brief: 闹钟，定时器
	建议少量使用底层提供的定时器. 因为api负责，大量使用容易出错，资源泄漏难跟踪。
	写了个应用层定时器，使用简单，问题容易跟踪。 需要底层一个定时器回调做驱动，

etc:
虚函数用法：
		class MyTimer : public Timer
		{
		public:
			//timeout会回调函数. timeout回调前变S_WAIT_START_TIMER 状态
			virtual void OnTimer(void *para)
			{
				UNIT_ASSERT((void *)11 == para);
			};
		};

		main(){
			MyTimer t1;
			t1.StartTimer(1, (void *)11);
		}

std::bind用法：

		void testFreeCb()
		{

		}

		main(){
			Timer t12;
			t1.StartTimer(1, testFreeCb);
		}
*/

#pragma once
#include "typedef.h"
#include "singleton.h"
#include "cd.h"
#include <time.h>
#include <vector>
#include <map>
#include <functional>

namespace su
{

	class Timer;

	namespace inner
	{
		//定时器控制数据
		struct CtrlData
		{
			time_t start_sec;
			uint32 interval;
			bool is_loop;  //true表示循环定时器
			Timer *pTimer;
			CtrlData()
				:start_sec(0)
				, interval(0)
				, is_loop(false)
				, pTimer(nullptr)
			{}
		};
	}
	/*
		定时器，注册回调函数，timeout调用函数。
        注意：定时器不合适设置太多，经验证明很多问题的。
	*/
	class TimeCallBack : public SingletonBase<TimeCallBack>
	{
		friend class Timer;
        typedef std::vector<inner::CtrlData> VecData;
		typedef std::multimap<time_t, inner::CtrlData> TimeMapData;   //到期绝对时间 map 数据   需要优化，频繁增加删除会有内存碎片
	public:
        //检测timeout事件，执行回调。（一般循环调用这个函数）
		void checkTimeOut();

        //清所有定时事件
		void clear();

        //获取等待到期的定时器数量
        uint32 GetTimeNum();

    private:
		//涉及指针接口私有化，防君子犯错.
		bool CreateTimer(Timer *pTimer, uint32 interval, bool is_loop= false);
		bool DelTimer(Timer *pTimer); //deattach and stop timer, 里面保证Timer指针删掉，不会野掉


	private:
        static const uint32 MAX_TIMER_NUMBER = 1000; //一般进程定时器超1000就是设计不好。
		TimeMapData m_time2data;
	};

	typedef std::function<void(void)> TimerCB;
	//里面做创建，销毁定时器，保证不泄露资源, 不会回调不存在的Timer
	class Timer
	{
		friend class TimeCallBack;
	public:
		Timer();
		virtual ~Timer();

		//timeout会回调函数. timeout回调前变S_WAIT_START_TIMER 状态
		virtual void OnTimer(void *para) {}; //传统方式重写实现

		//fun 启动timer,到时回调 Timer::OnTimer
		//para is_loop true表示循环定时器
		//return, true成功开始定时器，false 已经开始了，不需要设定(可以先stop,再start)
		bool StartTimer(uint32 interval, void *para = nullptr, bool is_loop = false);
		//@para const TimerCB &cb,  用std::bind绑定的函数
		bool StartTimer(uint32 interval, const TimerCB &cb, bool is_loop = false);
		//停止正在进行的定时器，
		//return, false 不需要停止. true 成功操作了停止
		bool StopTimer();

	private:
		void OnTimerCB();

	private:
		enum State
		{
			S_WAIT_START_TIMER,
			S_WAIT_TIME_OUT,
		};
		State m_state;
		TimerCB m_cb; //用std::bind方式绑定的回调函数
	};
	

}//namespace su