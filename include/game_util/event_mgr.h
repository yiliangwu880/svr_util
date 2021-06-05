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

	template<int ID, class MemFun, class T, class FunObj>
	struct BindPara
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindPara<1, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindPara<2, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindPara<3, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		}
	};
	template< class MemFun, class T, class FunObj>
	struct BindPara<4, MemFun, T, FunObj>
	{
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		}
	};

	//有 publish_subscribe.h，为什么提供局部对象管理的事件？
	//全局事件最安全好用，但没提供注销事件功能，没有注册成员函数功能。
	//注册接口提供 注册成员函数，编码工作更简洁。
	class EventMgr
	{
		template<class T>
		friend class EventCom;
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

		//one reg create one observer
		template<int ID>
		size_t GetObserverNum()
		{
			return m_idfuns[ID].vec.size();
		}
		template<int ID, class ... Args>
		void FireEvent(Args&& ... args)
		{
			using Fun = typename std::function<typename EventMgrTrait<ID>::Fun>;
			Observers &ss = m_idfuns[ID];
			if (ss.m_is_firing) //触发回调过程，禁止插入触发，避免复杂调用流程。
			{
				su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't recursion FireEvent");
				return;
			}
			ss.m_is_firing = true;
			for (Observer &v : ss.vec)
			{
				Fun *pFun = std::any_cast<Fun>(&v.funObj);

				(*pFun)(std::forward<Args>(args)...);
			}
			ss.m_is_firing = false;
		}
	private:
		//cb 为类成员函数。注意：需要用户保证事件回调时， ins不野。
		template<int ID, class MemFun, class T>
		void Reg(MemFun fun, T *ins)
		{
			using FunObj = std::function<typename EventMgrTrait<ID>::Fun>;
			
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
			FunObj funObj = BindPara<FunParaNum_<typename EventMgrTrait<ID>::Fun>::value, MemFun, T, FunObj>::bind(fun, ins);
			vec.emplace_back(Observer{ ins, fun, funObj });
		}

		template<int ID, class MemFun>
		bool Unreg(MemFun fun)
		{
			Observers &ss = m_idfuns[ID];
			if (ss.m_is_firing)
			{
				su::LogMgr::Ins().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "can't UnRegEvent when firing");
				return false;
			}
			std::vector<Observer> &vec = ss.vec;
			for (Observer &v : vec)
			{
				MemFun *pFun = std::any_cast<MemFun>(&v.memFun);
				if (nullptr != pFun && *pFun == fun)
				{
					v = vec.back();
					vec.erase(vec.end() - 1);
					return true;
				}
			}
			return false;
		}
		
		void UnregByIns(void *ins)
		{
			for (auto &vv: m_idfuns)
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
				for (size_t i : vecDelIdx)
				{
					vec[i] = vec.back();
					vec.erase(vec.end() - 1);
				}
			}
		
		}

	private:

	};


	//继承后，能让对象注册成员函数为事件函数
	//注意：m_owner必须拥有 this。this 是owner的组合对象（成员变量）.这样才能保证 this析构的时候，调用 m_owner 是有效的。
	template<class T>
	class EventCom
	{
		EventMgr &m_owner;
		//vector<pair<int, void* >> m_regData; //注册的ID,CB列表。用来析构的时候自动注销
	public:
		//用户注意：
		//通常一个类就构造函数写，减少出错的机会.和 比直接写注册传递指针 大大减少出错机会
		EventCom(EventMgr &owner)
			:m_owner(owner)
		{
		}
		~EventCom()
		{
			m_owner.UnregByIns(this);
		}
		//cb 为类成员函数。注意：需要用户保证事件回调时， ins不野。
		template<int ID, class MemFun>
		void Reg(MemFun fun)
		{
			m_owner.Reg<ID>(fun, (T *)this);
		}

		template<int ID, class MemFun>
		bool Unreg(MemFun fun)
		{
			return m_owner.Unreg<ID>(fun);
		}

	};


}