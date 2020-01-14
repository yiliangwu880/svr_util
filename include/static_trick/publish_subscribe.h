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
	缺点:实现不太好理解, 不能完成复用，需要修改一些自定义代码：EVENT_ID_INFO

适用:
	游戏很多子系统，互相调用需要订阅发布事件来解耦。

sample:
	参考 sample函数，或者如下：
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

	template<const int ID>
	struct EVENT_ID_INFO {
		using Fun = int;
	};

	template<>
	struct EVENT_ID_INFO<1> {
		using Fun = void(*)();
	};
	template<>
	struct EVENT_ID_INFO<2> {
		using Fun = void(*)(int i);
	};
	template<>
	struct EVENT_ID_INFO<11> {
		using Fun = void(*)();
	};
	template<>
	struct EVENT_ID_INFO<12> {
		using Fun = void(*)();
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
		template<const int ID>
		SubScribeSet<typename EVENT_ID_INFO<ID>::Fun> &GetChannel()
		{
			using Fun = typename EVENT_ID_INFO<ID>::Fun;
			static SubScribeSet<Fun> s;
			return s;
		}
	}

	//订阅
	//ID标识channel
	//对应发布的channel 由 TriggerEvent的 ID，Args 一起标识。
	template<const int ID>
	void RegEvent(typename EVENT_ID_INFO<ID>::Fun fun)
	{
		auto &ss = inner::GetChannel<ID>();
		if (ss.m_is_triggering)
		{
			su::LogMgr::Obj().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't RegEvent when triggering");
			return;
		}
		ss.m_funs.insert(fun);
		void(1);
	}


	template<typename Sig>
	struct get_;

	template<typename R, typename... Args>
	struct get_<R(*)(Args...)> {
		static const size_t  value = sizeof...(Args);
	};

	template<typename Sig>
	inline size_t FUN_PARA_SIZE(Sig) {
		return get_<Sig>::value;
	}


	template<const int ID>
	void UnRegEvent(typename EVENT_ID_INFO<ID>::Fun fun)
	{
		auto &ss = inner::GetChannel<ID>();
		ss.m_funs.erase(fun);
		void(1);
	}

	//发布
	//ID标识channel， 
	//有BUG，还不能用，只支持实参数为常量.原因待查
	template<const int ID, class ... Args>
	void TriggerEvent(Args&& ... args)
	{
		auto &ss = inner::GetChannel<ID>();
		if (ss.m_is_triggering) //触发回调过程，禁止插入触发，避免复杂调用流程。
		{
			su::LogMgr::Obj().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't recursion trigger");
			return;
		}
		ss.m_is_triggering = true;
		for (const auto &v : ss.m_funs)
		{
			printf("s2\n");
			v(std::forward<Args>(args)...);
		}
		ss.m_is_triggering = false;
	}

	//给用户参考用
	//////////////////////////////////////////////////////////////////////////
	namespace nouse
	{
		//用不了，参考这个思想吧。以后看能不能写出 一个参数可变的模板。
		template<const int ID, class ... Args>
		void PostTriggerEventRefer(Args&& ... args)
		{
		//	PostEvent &pe = inner::GetGlobalPostEvent();
			const std::function<void(void)> &f = std::bind(TriggerEvent<ID>, std::forward<Args>(args)...);
		//	pe.Add(f);
		}
		//参考用
		template<const int ID, class T1>
		void PostTriggerEventExample(T1 t1)
		{
			//PostEvent &pe = inner::GetGlobalPostEvent();
			auto f = [&]() {
				TriggerEvent<ID>(t1);
			};
			//pe.Add(f);
		}
		template<const int ID, class T1, class T2>
		void PostTriggerEventExample(T1 t1, T2 t2)
		{
			//PostEvent &pe = inner::GetGlobalPostEvent();
			auto f = [&]() {
				TriggerEvent<ID>(t1, t2);
			};
			//pe.Add(f);
		}

		template<const int ID, class T1, class T2, class T3>
		void PostTriggerEventExample(T1 t1, T2 t2, T3 t3)
		{
		}
	}
	//////////////////////////////////////////////////////////////////////////



} //end namespace su


//end file
