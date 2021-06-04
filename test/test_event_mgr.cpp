/*
*/

#include "game_util/event_mgr.h"
#include "unit_test.h"
#include "misc.h"
#include <iostream>
#include <map>
#include <string>
#include "stl_boost.h"
#include "static_reg.h"
#include "../src/log_def.h"

using namespace std;
using namespace su;
using namespace std::placeholders;


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


namespace su
{
	//template<>
	//struct EventMgrTraits<0> {
	//	using Fun = void();
	//};

	template<>
	struct EventMgrTraits<1> {
		using Fun = void(int i);
		static const int ParaNum = FunParaNum_<Fun>::value;
	};



	template<>
	struct EventMgrTraits<2> {
		using Fun = void(int a, int b);
		static const int ParaNum = 2;
	};
	template<>
	struct EventMgrTraits<3> {
		using Fun = void(int a, int b);
		static const int ParaNum = 2;
	};

	
	template< class MemFun, class T>
	struct BindPara<1, MemFun, T>
	{
		using FunObj = std::function<typename EventMgrTraits<1>::Fun>;
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1);
		}
	};

	template< class MemFun, class T>
	struct BindPara<2, MemFun, T>
	{
		using FunObj = std::function<typename EventMgrTraits<2>::Fun>;
		inline static FunObj bind(MemFun fun, T *ins)
		{
			return std::bind(fun, ins, std::placeholders::_1, std::placeholders::_2);
		}
	};

}
namespace
{

	class Player;

	struct EventCom
	{
		template<int i>
		void Reg()
		{
		}
		template<int i>
		void UnReg()
		{
		}
	};
	struct Sub1 : public EventCom
	{
		Player *m_owner;
		//void On1(int i)
		//{
		//}
		//void Regf()
		//{
		//	Reg<1>(On1);
		//}
		//void Unregf()
		//{
		//	UnReg<1>();
		//}
	};
	struct Sub2
	{
		Player *m_owner;
		void OnE0()
		{
			L_INFO("OnE0");
		}
		void OnE1(int i)
		{
			L_INFO("OnE1 %d",  i);

		}
		void OnE2(int a, int b)
		{
			L_INFO("OnE2 %d %d", a, b);

		}
		void reg();
	};

	class Player :  public EventMgr
	{
	public:
		Player()
		{
			m_Sub1.m_owner = this;
			m_Sub2.m_owner = this;
		}

		Sub1 m_Sub1;
		Sub2 m_Sub2;
		void f()
		{

		}
		template<int ID, class ... Args>
		void FireEvent2(Args&& ... args)
		{

		}
	};

	struct EventMgrTraits2 {
		using Fun = void();
	};

	void Sub2::reg()
	{
		m_owner->Reg<0>(&Sub2::OnE0, this); 
		m_owner->Reg<1>(&Sub2::OnE1, this);
		L_INFO("below line print error is ok");
		m_owner->Reg<1>(&Sub2::OnE1, this);
		m_owner->Reg<2>(&Sub2::OnE2, this);
	}
}

template<typename Sig>
struct get_;

template<typename R, typename... Args>
struct get_<R(*)(Args...)> {
	static size_t const value = sizeof...(Args);
};

template<typename Sig>
inline size_t get(Sig) {
	return get_<Sig>::value;
}

void fun(int, int) {}

void fun3(int, int, Player *) {}


template<typename Sig>
struct getg;
template<typename R, typename... Args>
struct getg<R(Args...)> {
	static size_t const value = sizeof...(Args);
};


UNITTEST(event_mgr)
{
	Player m;
	m.m_Sub2.reg();
	m.FireEvent<0>();
	m.FireEvent<1>(5);
	m.FireEvent<2>(5,6);
	L_INFO("end");


	L_INFO("fun para=%d", FunParaNum_<void(int i, int c)>::value); //获取参数数量
}