/*
author: yiliangwu880
可以从 git@github.com:yiliangwu880/svr_util.git 获取更多的参考。
发布订阅 ，publish/subscribe
目前TriggerEvent 不能用，不能识别变量类型
参考用，具体项目修改 TriggerEvent为更具体的变量类型。
特点：泛型编程
	订阅接口，任意类型
	发布接口，任意类型
	channel 由 事件id,函数类型一起标识
	用户用起来代码很简洁
	缺点:实现不太好理解

适用:
	游戏很多子系统，互相调用需要订阅发布事件来解耦。

sample:
	void OnAddMoney(Player &player, int num)
	{
		...
	}

	----------------------
	RegEvent<1>(OnAddMoney); //注册事件1回调函数

	----------------------
	Player player;
	TriggerEvent<1>(player, 10); //触发事件1

	----------延时调用，可以参考----------------------------------------

	PostTriggerEventExample<1>(player, 10); //延时触发事件1

	延时以后某个地方执行：
	DoPostEvent();

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "typedef.h"
#include <functional>
#include "../log_file.h"

#define CHECK_RECURSION_CALL //检查递归调用报错

namespace su
{
	using PostEventFun = std::function<void(void)>;
	class PostEvent
	{
		std::vector<PostEventFun> m_eventA;
		std::vector<PostEventFun> m_eventB;
		std::vector<PostEventFun> *m_add_event = &m_eventA;  //A B两个交互着用，一个用来接受新增，一个用来处理执行事件,避免各种嵌套调用，弄坏容器
		std::vector<PostEventFun> *m_do_event = &m_eventB;

	public:
		void Add(const PostEventFun &fun); //缓存事件，等以后Do批量执行
		void Do();
	};


	template<class Fun>
	struct SubScribeSet  
	{
		std::set<Fun> m_funs; //订阅channel的回调
		bool m_is_triggering = false; //true表示触发回调中
	};
	namespace inner
	{
		//静态保存订阅 channel
		//ID,Fun标识channel
		template<const int ID, class Fun>
		SubScribeSet<Fun> &GetChannel()
		{
			static SubScribeSet<Fun> s;
			return s;
		}
		//返回全局 PostEvent 对象，管理所有channel 的post event
		PostEvent &GetGlobalPostEvent();
	}

	//订阅
	//订阅channel 由 ID, Fun类型一起标识。
	//对应发布的channel 由 TriggerEvent的 ID，Agrs 一起标识。
	template<const int ID, class Fun>
	void RegEvent(Fun fun)
	{
		auto &ss = inner::GetChannel<ID, Fun>();
		if (ss.m_is_triggering)
		{
			su::LogMgr::Obj().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't RegEvent when triggering");
			return;
		}
		ss.m_funs.insert(fun);
		void(1);
	}

	template<const int ID, class Fun>
	void UnRegEvent(Fun fun)
	{
		auto &ss = inner::GetChannel<ID, Fun>();
		ss.m_funs.erase(fun);
		void(1);
	}

	//发布
	//ID,Fun标识channel， 
	//有BUG，还不能用，只支持实参数为常量.原因待查
	template<const int ID, class ... Agrs>
	void TriggerEvent(Agrs&& ... agrs)
	{
		using Fun = void(*)(Agrs...);
		auto &ss = inner::GetChannel<ID, Fun>();
		if (ss.m_is_triggering) //触发回调过程，禁止插入触发，避免复杂调用流程。
		{
			su::LogMgr::Obj().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't recursion trigger");
			return;
		}
		ss.m_is_triggering = true;
		for (const auto &v : ss.m_funs)
		{
			printf("s2\n");
			v(std::forward<Agrs>(agrs)...);
		}
		ss.m_is_triggering = false;
	}

	//用不了，参考这个思想吧。以后看能不能写出来。
	template<const int ID, class ... Agrs>
	void PostTriggerEventRefer(Agrs&& ... agrs)
	{
		PostEvent &pe = inner::GetGlobalPostEvent();
		const std::function<void(void)> &f = std::bind(TriggerEvent<ID>, std::forward<Agrs>(agrs)...);
		pe.Add(f);
	}
	//参考用
	template<const int ID>
	void PostTriggerEventExample(int i)
	{
		PostEvent &pe = inner::GetGlobalPostEvent();
		auto f = [&]() {
			TriggerEvent<ID>(i);
		};
		pe.Add(f);
	}
	template<const int ID>
	void PostTriggerEventExampleVoid()
	{
		PostEvent &pe = inner::GetGlobalPostEvent();
		auto f = [&]() {
			TriggerEvent<ID>();
		};
		pe.Add(f);
	}
	void DoPostEvent();


} //end namespace su


//end file
