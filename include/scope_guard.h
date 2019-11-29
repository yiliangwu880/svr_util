/*
//使用例子：

SCOPE_GUARD(delete point;);

 ScopeGuard sg([&] { exe; });
 sg.Dismiss();

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include <time.h>
#include "typedef.h"
#include <functional>

namespace su
{


	class ScopeGuard
	{
	private:
		std::function<void()> m_excute;
		bool                  m_is_dismissed;

	public:
		explicit ScopeGuard(std::function<void()> onExitScope)
			: m_excute(onExitScope)
			, m_is_dismissed(false)
		{ }

		~ScopeGuard()
		{
			if (!m_is_dismissed)
			{
				m_excute();
			}
		}

		void Dismiss()
		{
			m_is_dismissed = true;
		}

	private: // noncopyable
		ScopeGuard(ScopeGuard const&);
		ScopeGuard& operator=(ScopeGuard const&);
	};

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line) //多一层，避免__LINE__直接变化成字符串"__LINE__"

	//最简化用法
#define SCOPE_GUARD(exe)  ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)([&] { exe; })


} //end namespace su


//end file
