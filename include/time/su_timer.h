/*
	brief: 闹钟，定时器
	定时器代码注意，用户负责保证回调函数不引用野对象

etc:
驱动timer,定时调用：
		TimeDriver::Ins().checkTimeOut();
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

		void A::f()
		{

		}

		main(){
			Timer t12;
			t1.StartTimer(1, std::bind(&A::f, &a););
		}
*/

#pragma once
#include "../typedef.h"
#include "../singleton.h"
#include "su_cd.h"
#include <time.h>
#include <vector>
#include <map>
#include <functional>
#include "../cnt_typedef.h"
#include "su_timerDriver.h"

namespace su
{

	class Timer;
	class TimeDriver;

	using TimerCB = std::function<void(void)>;
	//特点：
	//析构的时候销毁定时器，保证不会回调不存在的Timer对象。
	//Noncopyable 不让复制，避免写出复杂易错代码。比如 vector<Timer> t; ，重分配内存就危险了。
	//注意： 
	//StartTimer 传入的 对象，生成期需要用户保证正确，不然会野！(转入方式包括 指针，引用， 函数对象保存的指针，引用 )
	//不要把Timer对象保存到单例，因为Timer引用了单例  TimeDriver::Ins()。 不然析构就会运行野对象。
	class Timer : private Noncopyable
	{
		friend class TimeDriver;


		enum State
		{
			S_WAIT_START_TIMER,
			S_WAIT_TIME_OUT,
		};
		State m_state = S_WAIT_START_TIMER;
		TimerCB m_cb = nullptr; //用std::bind方式绑定的回调函数
		inner::CtrlData m_ctrlData;

	public:
		Timer();
		virtual ~Timer();

		//timeout会回调函数. timeout回调前变S_WAIT_START_TIMER 状态
		virtual void OnTimer(void *para) {}; //重写timeout实现

		//fun 启动timer,到时回调 Timer::OnTimer
		//para is_loop true表示循环定时器
		//return, true成功开始定时器，false 已经开始了，不需要设定(可以先stop,再start)
		bool StartTimer(uint32 interval_sec, void *para = nullptr, bool is_loop = false);
		//@brief 回调方式设置timer
		//@para const TimerCB &cb,  用std::bind绑定的函数
		bool StartTimer(uint32 interval_sec, const TimerCB &cb, bool is_loop = false);

		//简化std::bind书写。需要 _1,_2等不支持，还是手动敲出来吧
		template< class MemFun, class T>
		inline bool StartTimer(uint32 interval_sec, MemFun fun, T *ins, bool is_loop = false)
		{
			return StartTimer(interval_sec, std::bind(fun, ins), is_loop);
		}

		//停止正在进行的定时器，
		//return, false 不需要停止. true 成功操作了停止
		bool StopTimer();

	private:
		void OnTimerCB();

	private:
	};
	

	using IncTimerCB = std::function<bool(void)>;
	//递增定时器，通常断线重连需要用。
	class IncTimer
	{
	public:
		IncTimer()
			:m_vi_idx(0)
		{}
		//启动定时器，按照间隔列表 逐次递增间隔。最后一个间隔做循环定时。
		//直至回调函数返回true或者调用Stop()，才结束定时器。
		//@para const TimerCB &cb,  用std::bind绑定的函数
		//@para VecUint32 vec_interval, 间隔列表
		//@return 非法参数就返回false,重复Start返回false
		bool Start(const IncTimerCB &cb, const VecUint32 &vec_interval);
		//缺省时间间隔列表： 10,60,60*5,60*30
		bool Start(const IncTimerCB &cb);
		bool Stop();

	private:
		void TimerCb();
	private:
		Timer m_timer;
		IncTimerCB m_cb;
		VecUint32 m_vi; //vec_interval 时间间隔
		uint32 m_vi_idx;
	};

}//namespace su