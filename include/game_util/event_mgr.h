//）防递归
//）注册对象销毁，自动注销
//

#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <any>
#include <set>
#include "typedef.h"
#include <functional>
#include <tuple>
#include "../log_file.h"
#include "static_reg.h"

namespace su
{
	//定义事件ID 关联 接收函数参数列表
	template<int ID>
	struct EventMgrTraits {
		using Fun = void();
	};


	template<typename Sig>
	struct FunParaNum_;

	template<typename R, typename... Args>
	struct FunParaNum_<R(Args...)> {
		static size_t const value = sizeof...(Args);
	};
	template<typename Sig>
	inline size_t FunParaNum(Sig) {
		return FunParaNum_<Sig>::value;
	}


	template<int ID, class MemFun, class T, class FunObj>
	struct BindPara2
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindPara2<1, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindPara2<2, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2);
		}
	};
	//有 publish_subscribe.h，为什么提供局部对象管理的事件？
	//全局事件最安全好用，但没提供注销事件功能，没有注册成员函数功能。
	//注册接口提供 注册成员函数，编码工作更简洁。
	class EventMgr
	{
		struct Observer
		{
			void *ins = nullptr;
			std::any memFun; //成员函数指针 ，例如 void (Player::*)();
			std::any funObj; //函数对象 function<xx>
		};
		struct Observers
		{
			bool m_is_firing = false;
			std::vector<Observer> vec;
		};
		std::unordered_map<int, Observers> m_idfuns;
		bool m_isForbiding = false;
	public:
		~EventMgr()
		{
			m_isForbiding = true;
		}

		//template<int ID, class MemFun, class T>
		//void Reg2(MemFun fun, T *ins)
		//{
		//	using Fun = std::function<typename EventMgrTraits<ID>::Fun>;

		//	Observers ss = m_idfuns[ID];
		//	if (ss.m_is_firing)
		//	{
		//		su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't RegEvent when firing");
		//		return;
		//	}
		//	std::vector<Observer> &vec = ss.vec;
		//	for (Observer &v : vec)
		//	{
		//		MemFun *pFun = std::any_cast<MemFun>(&v.memFun);
		//		if (v.ins == ins && *pFun == fun)
		//		{
		//			su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "repeated reg");
		//			return;
		//		}
		//	}
		//	//Fun funObj = &StaticEventFun<ID>;

		//	//vec.emplace_back({ ins, fun, funObj });
		//}

	
		//cb 为类成员函数。注意：需要用户保证事件回调时， ins不野。
		template<int ID, class MemFun, class T>
		void Reg(MemFun fun, T *ins)
		{
			using FunObj = std::function<typename EventMgrTraits<ID>::Fun>;
			
			Observers &ss = m_idfuns[ID];
			if (ss.m_is_firing)
			{
				su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't RegEvent when firing");
				return;
			}
			std::vector<Observer> &vec = ss.vec;
			for (Observer &v : vec)
			{
				MemFun *pFun = std::any_cast<MemFun>(&v.memFun);
				if (v.ins == ins && *pFun == fun)
				{
					su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "repeated reg");
					return;
				}
			}
			FunObj funObj = BindPara2<FunParaNum_<typename EventMgrTraits<ID>::Fun>::value, MemFun, T, FunObj>::bind(fun, ins);
			vec.emplace_back(Observer{ ins, fun, funObj });
		}

		template<int ID, class MemFun>
		bool UnRegEvent(MemFun fun)
		{
			Observers ss = m_idfuns[ID];
			if (ss.m_is_firing)
			{
				su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't UnRegEvent when firing");
				return false;
			}
			std::vector<Observer> &vec = ss.vec;
			for (Observer &v : vec)
			{
				MemFun *pFun = std::any_cast<MemFun>(&v.memFun);
				if (*pFun == fun)
				{
					v = vec.back();
					vec.erase(vec.end() - 1);
					return true;
				}
			}
			return false;
		}

		template<int ID, class ... Args>
		void FireEvent(Args&& ... args)
		{
			using Fun = typename std::function<typename EventMgrTraits<ID>::Fun>;
			Observers &ss = m_idfuns[ID];
			if (ss.m_is_firing) //触发回调过程，禁止插入触发，避免复杂调用流程。
			{
				su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't recursion FireEvent");
				return;
			}
			ss.m_is_firing = true;
			for ( Observer &v : ss.vec)
			{
				 Fun *pFun = std::any_cast<Fun>(&v.funObj);

				(*pFun)(std::forward<Args>(args)...);
			}
			ss.m_is_firing = false;
		}
	private:

	};
#if 0

	enum InsEventId
	{
		ChangeHp,
		InterUp,//打断技能
		ChangeXY,

	};




	template<>
	struct EventTraits< ChangeHp> {
		using Fun std::function<void(int &update)>;
		template<class T, class C>
		Fun Bind(T cb, C *m_owner)
		{
			return std::bind(cb, m_owner, _1);
		}
	};

	//继承后，能让对象注册成员函数为事件函数
	//注意：m_owner必须拥有 this。这样才能保证 this析构的时候，调用 m_owner 是有效的。
	class EventCom
	{
		EventMgr m_owner;
		vector<pair<int, void* >> m_regData; //注册的ID,CB列表。用来析构的时候自动注销
	public:
		//用户注意：this 是owner的组合对象（成员变量）
		//通常一个类就构造函数写，减少出错的机会.和 比直接写注册传递指针 大大减少出错机会
		EventCom(EventMgr &owner);
		~EventCom()
		{
			for (auto &v : m_regData)
			{
				m_owner.UnReg(v->first, v->second);
			}
		}
		//Reg 注册this 成员函数到  this owner 事件管理器。能保证事件回调不野
		template<class T>
		void Reg(int ID, T cb)
		{
			m_owner.Reg(ID, cb, this);
		}

		void UnReg(int ID, T cb)
		{
			m_owner.UnReg(ID, cb);
		}
		virtual void Init(cfg);
	};

	//有 publish_subscribe.h，为什么提供局部对象管理的事件？
	//全局事件最安全好用，但没提供注销事件功能，没有注册成员函数功能。
	//注册接口提供 注册成员函数，编码工作更简洁。
	class EventMgr
	{
		struct Observer
		{
			void *ins;
			std::any memFun; //成员函数指针 ，例如 void (Player::*)();
			std::any funObj; //函数对象 function<xx>
		};
		template<class Fun>
		struct Observers
		{
			std::set<void*> m_funs;//void* 表示任意类型回调函数
			bool m_is_firing = false; //true表示触发回调中
			std::set<void*> m_cacheRegFuns; //缓存注册，注销函数。等事件操作完成后执行
			std::set<void*> m_cacheUnRegFuns;

		};
		std::unordered_map<int, std::vector<Observer> > m_idfuns;

		std::unordered_map<void *, void *> m_fun2funObj; //成员函数地址 2 函数对象地址
		bool m_isForbiding = false;
	public:
		~EventMgr()
		{
			m_isForbiding = true;
		}
		template<int ID, class ... Args>
		void FireEvent(Args&& ... args)
		{
			if (Forbiding())
			{
				return;
			}
			auto &ss = inner::GetChannel<ID>();

			ss.m_is_firing = true;
			for (const auto &v : ss.m_funs)
			{
				v(std::forward<Args>(args)...);//这里不好通用地调用类成员函数指针（理论上可以，但是需要研究复杂的实现），所以用函数对象实现比较保险。
			}
			ss.m_is_firing = false;
			if (!ss.m_cacheRegFuns.empty())
			{
				ss.m_funs.insert(ss.m_cacheRegFuns.begin(), ss.m_cacheRegFuns.end());
			}
			if (!ss.m_cacheRegFuns.empty())
			{
				ss.m_funs.insert(ss.m_cacheRegFuns.begin(), ss.m_cacheRegFuns.end());
			}
		}
	private:
		//cb 为类成员函数。注意：需要用户保证事件回调时， ins不野。
		template<int ID, class MemFun>
		void Reg(MemFun fun, void *ins)
		{
			using Fun = std::function<EventMgrTraits<ID>::Fun>;
			std::vector<Observer> &vec = m_idfuns[ID];
			for (Observer &v : vec)
			{
				MemFun *pFun = std::any_cast<MemFun>(&v.memFun);

				if (v.ins == ins && *pFun == fun)
				{
					//repeated reg
					return;
				}
			}
			Fun funObj = std::Bind(fun, ins);
			Observer obs = { ins, fun, funObj };
			vec.push(obs);
		}

		template<int ID, class T>
		void UnReg(T cb)
		{
			mgr.UnRegEvent(ID, m_fun2funObj[cb]);
		}
		//具体注册， fun是强类型检查
		template<const int ID>
		void * RegEvent(typename EventTraits<ID>::Fun fun)
		{
			using FunObj = typename EventTraits<ID>::Fun;
			FunObj *p = new FunObj(fun)
				auto &ss = m_idfuns[ID];
			if (ss.m_is_firing) //触发回调过程，延后插入触发
			{
				ss.m_cacheRegFuns.insert(p);
				return;
			}
			ss.m_funs.insert(p);
			return p;
		}
		void UnRegEvent(int ID, void *p)
		{
			auto &ss = m_idfuns[ID];
			if (ss.m_is_firing) //触发回调过程，延后插入触发
			{
				ss.m_cacheUnRegFuns.erase(p);
				return;
			}
			ss.m_funs.erase(p);
		}
	};



	template<class T>
	class EventCom
	{
		EventMgr mgr;
	public:

		void Reg<ChangeHp>(T cb, C *m_owner)
		{
			auto ff = EventTraits<ChangeHp>::Bind(cb, m_owner);
			mgr.RegEvent<ChangeHp>(ff);
		}
	private:
		void EventCb();
	};


#endif

}