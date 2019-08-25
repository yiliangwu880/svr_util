/*
author: YiliangWu
���
)id������
)score guard

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include <time.h>
#include "typedef.h"
#include <functional>

#define UNUSED(x) (void)(x)

namespace su
{

//Ψһid  ��32��timestamp, ��32��������  1���ڣ���������4�ڣ����ܱ�֤Ψһ
//һ�㼸�д��룬�ظ���4�ڴΣ���Ҫʮ�����ˡ� ����ʵ����������ظ�
//32λ��ʾʱ���������һ�ٶ��겻�ظ���
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


//ʹ�����ӣ� SCOPE_GUARD(delete point;);
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
//����÷�
#define SCOPE_GUARD(exe)  ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)([&] { exe; })


} //end namespace su


//end file
