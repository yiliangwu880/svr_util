/*
	brief: 闹钟，定时器
	建议少量使用底层提供的定时器. 因为api复杂，大量使用容易出错，资源泄漏难跟踪。
	写了个应用层定时器，使用简单，问题容易跟踪。 需要底层一个定时器回调做驱动，

etc:
驱动timer,定时调用：
		TimeDriver::Obj().checkTimeOut();
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
#include "../typedef.h"
#include "../singleton.h"
#include "su_cd.h"
#include <time.h>
#include <vector>
#include <map>
#include <functional>
#include "../cnt_typedef.h"

namespace su
{

	class Timer;

	namespace inner
	{
		//定时器控制数据
		struct CtrlData
		{
			time_t start_sec;
			uint32 interval_sec;
			bool is_loop;  //true表示循环定时器
			Timer *pTimer;
			CtrlData()
				:start_sec(0)
				, interval_sec(0)
				, is_loop(false)
				, pTimer(nullptr)
			{}
		};
	}
	/*
		定时器，注册回调函数，timeout调用函数。
        注意：定时器不合适设置太多，经验证明很多问题的。
	*/
	class TimeDriver : public Singleton<TimeDriver>
	{
		friend class Timer;
        using VecData = std::vector<inner::CtrlData>;
		using TimeMapData = std::multimap<time_t, inner::CtrlData>;   //到期绝对时间 map 数据   需要优化，频繁增加删除会有内存碎片


	public:
		~TimeDriver();
        //检测timeout事件，执行回调。（一般循环调用这个函数）
		void CheckTimeOut();

        //清所有定时事件
		void Clear();

        //获取等待到期的定时器数量
        uint32 GetTimeNum();

    private:
		//涉及指针接口私有化，防君子犯错.
		bool NewTimer(Timer *pTimer, uint32 interval_sec, bool is_loop= false);
		bool DelTimer(Timer *pTimer); //deattach and stop timer, 里面保证Timer指针删掉，不会野掉


	private:
        static const uint32 MAX_TIMER_NUMBER = 1000; //一般进程定时器超1000就是设计不好。
		TimeMapData m_time2data;
	};

	using TimerCB = std::function<void(void)>;
	//里面做创建，销毁定时器，保证不泄露资源, 不会回调不存在的Timer
	//注意： StartTimer传入的 指针，引用 对象，生成期需要用户保证正确，不然会野！
	class Timer
	{
		friend class TimeDriver;
	public:
		Timer();
		virtual ~Timer();

		//timeout会回调函数. timeout回调前变S_WAIT_START_TIMER 状态
		virtual void OnTimer(void *para) {}; //重写timeout实现

		//fun 启动timer,到时回调 Timer::OnTimer
		//para is_loop true表示循环定时器
		//return, true成功开始定时器，false 已经开始了，不需要设定(可以先stop,再start)
		bool StartTimer(uint32 interval_sec, void *para = nullptr, bool is_loop = false);
		//@para const TimerCB &cb,  用std::bind绑定的函数
		bool StartTimer(uint32 interval_sec, const TimerCB &cb, bool is_loop = false);
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