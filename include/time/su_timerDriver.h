#pragma once
#include "../typedef.h"
#include "../singleton.h"
#include "su_cd.h"
#include <time.h>
#include <vector>
#include <map>
#include <functional>
#include "../cnt_typedef.h"
#include "minheap.h"

namespace su
{

	class Timer;
	namespace inner
	{
		//定时器控制数据
		struct CtrlData : public MinHeapNodeBase<CtrlData>
		{
			time_t start_sec;
			time_t end_sec;
			uint32 interval_sec;
			bool is_loop;  //true表示循环定时器
			Timer &m_owner;
			CtrlData(Timer &timer)
				:start_sec(0)
				, interval_sec(0)
				, is_loop(false)
				, m_owner(timer)
			{}
			bool IsLess(const CtrlData &other) {
				return end_sec < other.end_sec;
			}
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
		bool NewTimer(Timer &timer, uint32 interval_sec, bool is_loop = false);
		bool DelTimer(Timer &timer); //deattach and stop timer, 里面保证Timer指针删掉，不会野掉


	private:
		MinHeap< inner::CtrlData> m_all;
	};

}