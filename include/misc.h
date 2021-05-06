/*
参考 SnowFlake算法 重新做一个吧
author: YiliangWu
杂项：
)id生成器
)score guard

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include <time.h>
#include "typedef.h"
#include <functional>
#include <memory>

#define UNUSED(x) (void)(x)  //无用代码，用来去编译警告

namespace su
{

	//唯一id  高32用timestamp, 低32用自增。 
	//保证唯一条件： 1）使用少于一百多年。2）1秒内，产生少于4亿。
	//一般几行代码，重复跑4亿次，都要十几秒了。 所以实际情况不会重复
	//32位表示时间戳，够用一百多年不重复。
	class IdCreater
	{
	public:
		IdCreater()
		{
			time_t sec;
			time(&sec);
			m_lAdapativeTime = (uint32)sec;
		}
		uint64 CreateId()
		{
			time_t sec;
			time(&sec);
			m_seed++;
			uint64 id = sec << 32;
			id = id | m_seed;
			return id;
		}
		//更优化计算，参考用
		//      *  自适应时间根据当前时间进行修正，初始为当前时间
		//  *  若自适应时间滞后于当前时间，说明创建UID速度较慢，每次创建UID时修正靠近当前时间
		// //  *  若创建UID速度较快，例如在一秒内循环序号已经循环一次了(少见情况)，自适应时间将会快于当前时间
		uint64 CreateId2()
		{
			return 0; //unfinish
			time_t iNowTime;
			time(&iNowTime);
			m_seed++;

			if (iNowTime > m_lAdapativeTime)
			{
				m_lAdapativeTime++;
			}

			if (m_seed == 0)
			{
				m_lAdapativeTime++;
				if (m_lAdapativeTime > iNowTime)
				{
					//LOG_DBG_SEC("gid too fast, type:%d, time exceed:%ld", m_iType, m_lAdapativeTime- iNowTime);
				}
			}

			uint64 id = 0;//m_lAdapativeTime << 32; unfinish
			id = id | m_seed;


			return id;
		}
	private:
		uint32 m_seed = 0;
		uint32 m_lAdapativeTime = 0;
	};

	template<class T>
	struct WeakPtr {
		WeakPtr()
			:m_sharePtr((T*)this, [](T *) {})
		{

		}
		std::shared_ptr<T> m_sharePtr;
		std::weak_ptr<T> GetWeakPtr()
		{
			return m_sharePtr;
		}
	};
} //end namespace su


//end file
