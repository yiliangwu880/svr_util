/*
事件注册侦听。使用写法简洁。事件参数强类型

有 publish_subscribe.h，为什么提供局部对象管理的事件？
全局事件最安全好用，但没提供注销事件功能。
注册接口提供 注册成员函数，编码工作更简洁。
适用场景：一个游戏player对象内部很多子组件，组件需要事件解耦。

例子：
#include "game_util/event_mgr.h"
	namespace su //必须要定义名空间
	{
		template<>
		struct EventMgrTrait<1> {
			using Fun = void(int i);
		};
		template<>
		struct EventMgrTrait<2> {
			using Fun = void(int a, int b);
		};
		template<>
		struct EventMgrTrait<3> {
			using Fun = void(int a, int b);
		};
	}
	class Player;
	struct Sub1 : public EventCom<Sub1>, B1 , B2
	{
		Player &m_owner;
		Sub1(Player &owner);

		void OnE1(int i)
		{
		}
	};
	struct Sub2 : public EventCom<Sub2>, B1
	{
		....
		void OnE0()
		{
		}
	};
	class Player :  public EventMgr, B1
	{
	public:
		Sub1 m_Sub1;
		Sub2 m_Sub2;
	};

	{
		Player m;
		m.m_Sub1.Reg<1>(&Sub1::OnE0);
		m.m_Sub2.Reg<0>(&Sub2::OnE1);

		m.FireEvent<0>();
		m.FireEvent<1>(5);


		m.m_Sub2.Unreg<1>(&Sub2::OnE1);
	}

*/
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


//定义事件ID 关联 接收函数参数列表

namespace su
{
	template<int ID>
	struct EventMgrTrait {
		using Fun = void();
	};
	//根据函数类型，获取参数数量
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

	//根据参数数量，特例化不同参数数量的bind用法
	template<int PARA_NUM, class MemFun, class T, class FunObj>
	struct BindFun
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindFun<1, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindFun<2, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindFun<3, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindFun<4, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		}
	};
	
	class EventMgr
	{
		template<class T>
		friend class EventCom;

		struct Observer
		{
			void *ins = nullptr;//对象指针
			std::any memFun; //成员函数指针 ，例如 void (Player::*)();
			std::any funObj; //函数对象 function<xx> 类型
		};
		struct Observers
		{
			bool m_is_firing = false;
			std::vector<Observer> vec;
		};

		std::unordered_map<int, Observers> m_id2obss= std::unordered_map<int, Observers>(64); //一般事件ID从1开始，不需要太多，节省内存

	public:
		//one reg create one observer
		template<int ID>
		size_t GetObserverNum()
		{
			return m_id2obss[ID].vec.size();
		}
		template<int ID, class ... Args>
		void FireEvent(Args&& ... args)
		{
			using FunObj = std::function<typename EventMgrTrait<ID>::Fun>;
			Observers &ss = m_id2obss[ID];
			if (ss.m_is_firing) //触发回调过程，禁止插入触发，避免复杂调用流程。
			{
				LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't recursion FireEvent");
				return;
			}
			ss.m_is_firing = true;
			for (Observer &v : ss.vec)
			{
				FunObj *pFun = std::any_cast<FunObj>(&v.funObj);
				(*pFun)(std::forward<Args>(args)...);
			}
			ss.m_is_firing = false;
		}

	private:
		//注意：需要用户保证事件回调时， ins不野。
		template<int ID, class MemFun, class T>
		void Reg(MemFun fun, T *ins)
		{
			using FunObj = std::function<typename EventMgrTrait<ID>::Fun>;
	
			Observers &ss = m_id2obss[ID];
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
			constexpr int paraNum = FunParaNum_<typename EventMgrTrait<ID>::Fun>::value;
			FunObj funObj = BindFun<paraNum, MemFun, T, FunObj>::bind(fun, ins);
			vec.emplace_back(Observer{ ins, fun, funObj });
		}

		template<int ID, class MemFun, class T>
		bool Unreg(MemFun fun, T *ins)
		{
			Observers &ss = m_id2obss[ID];
			if (ss.m_is_firing)
			{
				su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't UnRegEvent when firing");
				return false;
			}
			std::vector<Observer> &vec = ss.vec;
			for (Observer &v : vec)
			{
				if (v.ins == ins)//先用对象快速筛选
				{
					MemFun *pFun = std::any_cast<MemFun>(&v.memFun);
					if (nullptr != pFun && *pFun == fun)
					{
						v = vec.back();
						vec.erase(vec.end() - 1);
						return true;
					}
				}
			}
			return false;
		}
		
		void UnregByIns(void *ins)
		{
			for (auto &vv: m_id2obss)
			{
				Observers &ss = vv.second;
				if (ss.m_is_firing)
				{
					su::LogMgr::Ins().Printf(su::LL_FATAL, __FILE__, __LINE__, __FUNCTION__, "can't UnRegEvent when firing");
					return;
				}
				std::vector<Observer> &vec = ss.vec;
				std::vector<size_t> vecDelIdx;
				size_t idx = 0;
				for (Observer &v : vec)
				{
					if (ins == v.ins)
					{
						vecDelIdx.push_back(idx);
					}
					idx++;
				}
				for (auto it = vecDelIdx.rbegin(); it != vecDelIdx.rend(); ++it)//have bug when del more
				{
					size_t i = *it;
					//printf("vec size %ld, del i=%ld\n", vec.size(), i);
					vec[i] = vec.back();
					vec.erase(vec.end() - 1);
				}
			}
		}
	};


	//继承后，能让对象注册成员函数为事件函数
	//注意：m_mgr必须拥有 this。this 是m_mgr的组合对象（成员变量）.这样才能保证 this析构的时候，调用 m_owner 是有效的。
	template<class T>
	class EventCom
	{
		EventMgr &m_mgr;
	public:
		EventCom(EventMgr &mgr)
			:m_mgr(mgr)
		{
		}
		~EventCom()
		{
			m_mgr.UnregByIns((T *)this);
		}
		//cb 为类成员函数。注意：需要用户保证事件回调时， ins不野。
		template<int ID, class MemFun>
		void Reg(MemFun fun)
		{
			//如果T有虚函数 this > (T *)this 8个字节左右
			m_mgr.Reg<ID>(fun, (T *)this);
		}

		template<int ID, class MemFun>
		bool Unreg(MemFun fun)
		{
			return m_mgr.Unreg<ID>(fun, (T *)this);
		}
	};


}