/*
author: yiliangwu880
可以从 git@github.com:yiliangwu880/svr_util.git 获取更多的参考。
事件，发布订阅 ，publish/subscribe
参考用，具体项目需要新增 多个 EventTraits 定义
特点：泛型编程
	事件接口
	事件ID和事件函数类型绑定一起定义。
	channel 由 事件id标识
	用户用起来代码很简洁
	注册事件入口不提供注册对象。 避免对象生存期的控制，减少野指针问题。
	缺点: 奇技淫巧. 需要修改一些自定义代码：EventTraits
适用:
	游戏很多子系统，互相调用需要事件来解耦。 
	好处： 各子系统开发不用互相关注代码，都关注中介作用的事件管理系统就可以了。

设计思想：
	事件比较常见和便捷的方式可以这样写：
		mgr.RegEvent(eventId, &ins, &Ins::fun); --或者std::bind，函数对象等。共同点都是把对象注册导事件管理器。
		mgr.Fire(eventId)			--触发事件回调函数。
	这个有个容易出错的地方，注册的事件对象生存期怎么维护？一不小心就导致 mgr.Fire 调用了野指针了。合适有CG的语言。
	所有这里设计注重稳健的代码。不考虑灵活性，只提供注册函数指针。 

code example:
	
	//声明事件参数列表
	----------------------
	template<>
	struct EventTraits<1> {
		using Fun = void(*)(Player &player, int num);
	};	

	//注册事件1回调函数。 为什么放这里：优点，不用去跳别的地方写代码，提高些写代码效率。 缺点，浏览代码不好找到统一注册的地方（利用vs查找引用功能可以解决）。
	----------------------
	STATIC_RUN(RegEvent<1>(OnAddMoney);)   -- 可考虑宏 REG_EVENT(1, OnAddMoney)
	void OnAddMoney(Player &player, int num)
	{
		...
	}

	//触发事件1
	----------------------
	Player player;
	FireEvent<1>(player, 10);


	----------延时调用，可以参考----------------------------------------

	PostFireEventExample<1>(player, 10); //延时触发事件1

	延时以后某个地方执行：注意延时后，你的传入对象生存期是否有效！！
	DoPostEvent();


*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "typedef.h"
#include <functional>
#include <tuple>
#include "../log_file.h"
#include "static_reg.h"

namespace su
{
	using PostEventFun = std::function<void(void)>;
	//注意：延时调用容易引用野对象。用户需要正确控制对象的生存期。
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

	//定义事件ID 关联 接收函数参数列表
	template<int ID>
	struct EventTraits {
		using Fun = int;
	};

	//template<>
	//struct EventTraits<1> {
	//	using Fun = void(*)();
	//};
	//template<>
	//struct EventTraits<2> {
	//	using Fun = void(*)(int i);
	//};
	//template<>
	//struct EventTraits<11> {
	//	using Fun = void(*)();
	//};
	//template<>
	//struct EventTraits<12> {
	//	using Fun = void(*)();
	//};


	template<class Fun>
	struct SubscribeSet  
	{
		std::set<Fun> m_funs; //订阅channel的回调集合
		bool m_is_firing = false; //true表示触发回调中
	};

	namespace inner//用户不需要访问，不能是匿名。匿名会导致每个cpp文件独立生成不同对象
	{
		//ID标识不同channel
		template<int ID>
		SubscribeSet<typename EventTraits<ID>::Fun> &GetChannel()
		{
			using Fun = typename EventTraits<ID>::Fun;
			static SubscribeSet<Fun> s;
			return s;
		}
	}

	//订阅
	template<const int ID>
	void RegEvent(typename EventTraits<ID>::Fun fun)
	{
		auto &ss = inner::GetChannel<ID>();
		if (ss.m_is_firing)
		{
			su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't RegEvent when firing");
			return;
		}
		ss.m_funs.insert(fun);
	}


	template<const int ID>
	void UnRegEvent(typename EventTraits<ID>::Fun fun)
	{
		auto &ss = inner::GetChannel<ID>();
		if (ss.m_is_firing)
		{
			su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't UnRegEvent when firing");
			return;
		}
		ss.m_funs.erase(fun);
	}

	//发布
	//ID标识channel， 非类型形参 只支持实参为常量
	template<int ID, class ... Args>
	void FireEvent(Args&& ... args)
	{
		auto &ss = inner::GetChannel<ID>();
		if (ss.m_is_firing) //触发回调过程，禁止插入触发，避免复杂调用流程。
		{
			su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't recursion trigger");
			return;
		}
		ss.m_is_firing = true;
		for (const auto &v : ss.m_funs)
		{
			v(std::forward<Args>(args)...);
		}
		ss.m_is_firing = false;
	}

	//给用户参考用
	//////////////////////////////////////////////////////////////////////////
	namespace nouse
	{
		//用不了，参考这个思想吧。以后看能不能写出 一个参数可变的模板。
		template<const int ID, class ... Args>
		void PostFireEventRefer(Args&& ... args)
		{
		//	PostEvent &pe = GetGlobalPostEvent();
			const std::function<void(void)> &f = std::bind(FireEvent<ID>, std::forward<Args>(args)...);
		//	pe.Add(f);
		}
		//参考用
		template<const int ID, class T1>
		void PostFireEventExample(T1 t1)
		{
			//PostEvent &pe = GetGlobalPostEvent();
			auto f = [&]() {
				FireEvent<ID>(t1);
			};
			//pe.Add(f);
		}
		template<const int ID, class T1, class T2>
		void PostFireEventExample(T1 t1, T2 t2)
		{
			//PostEvent &pe = GetGlobalPostEvent();
			auto f = [&]() {
				FireEvent<ID>(t1, t2);
			};
			//pe.Add(f);
		}

		template<const int ID, class T1, class T2, class T3>
		void PostFireEventExample(T1 t1, T2 t2, T3 t3)
		{
		}
	}
	//////////////////////////////////////////////////////////////////////////



} //end namespace su


//end file
