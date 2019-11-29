/*
author: yiliangwu880
可以从 git@github.com:yiliangwu880/svr_util.git 获取更多的参考。
发布订阅 ，publish/subscribe
特点：泛型编程
	订阅接口，任意类型
	发布接口，任意类型。
	channel 由 事件id,函数类型一起标识
 
*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "typedef.h"
#include <functional>

namespace su
{
	//静态保存订阅
	template<const int ID, class Fun>
	std::set<Fun> &GetSubScribeSet()
	{
		static std::set<Fun> s;
		return s;
	}

	//订阅
	//订阅channel 由 ID, Fun类型一起标识。
	//对应发布的channel 由 TriggerEvent的 ID，Agrs 一起标识。
	template<const int ID, class Fun>
	void RegEvent(Fun fun)
	{
		auto &ss = GetSubScribeSet<ID, Fun>();
		ss.insert(fun);
		void(1);
	}

	//发布
	template<const int ID>
	void TriggerEvent()
	{
		using Fun = void(*)(void);
		auto &ss = GetSubScribeSet<ID, Fun>();
		for (auto &v : ss)
		{
			v();
		}
	}

	template<const int ID, class ... Agrs>
	void TriggerEvent(Agrs&& ... agrs)
	{
		using Fun = void(*)(Agrs...);
		auto &ss = GetSubScribeSet<ID, Fun>();
		for (auto &v : ss)
		{
			v(std::forward<Agrs>(agrs)...);
		}
	}


} //end namespace su


//end file
