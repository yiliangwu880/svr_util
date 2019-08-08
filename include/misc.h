/*
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

#define UNUSED(x) (void)(x)

namespace su
{

//唯一id  高32用timestamp, 低32用自增。  1秒内，产生少于4亿，就能保证唯一
//一般几行代码，重复跑4亿次，都要十几秒了。 所以实际情况不会重复
//32位表示时间戳，够用一百多年不重复。
class IdCreater  
{
public:
	uint64 CreateId()
	{
		time_t sec;
		time(&sec);
		m_seed++;
		uint64 id = sec << 32;
		id = id | m_seed;
		return id;
	}

private:
	uint32 m_seed = 0;
};


//使用例子： SCOPE_GUARD(delete point;);
// ScopeGuard sg([&] { exe; });
//sg.Dismiss();
class ScopeGuard
{
public:
	explicit ScopeGuard(std::function<void()> onExitScope)
		: onExitScope_(onExitScope)
		, dismissed_(false)
	{ }

	~ScopeGuard()
	{
		if (!dismissed_)
		{
			onExitScope_();
		}
	}

	void Dismiss()
	{
		dismissed_ = true;
	}

private:
	std::function<void()> onExitScope_;
	bool dismissed_;

private: // noncopyable
	ScopeGuard(ScopeGuard const&);
	ScopeGuard& operator=(ScopeGuard const&);
};

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)
//最简化用法
#define SCOPE_GUARD(exe)  ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)([&] { exe; })


} //end namespace su


//end file
