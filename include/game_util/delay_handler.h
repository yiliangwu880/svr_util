/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/svr_util.git
brief: 延时处理逻辑， 
比如:
为了避免处理离线和在线玩家的相同功能写不同函数流程。统一入口处理。
例子：
	class MyOptMgr: public DelayOptMgr<MyTarget, uint64>
	{
	public:
		virtual MyTarget *OnFindTarget(uint64 target_id)				//1 实现查找内存目标	
		{
			...
		}

		virtual void OnMissTarget(uint64 target_id)						//2 实现，请求读档。  (内存没有对象时会调用）
		{
			...
		}
	};

	MyOptMgr mgr;

	mgr.OptTarget(id, target);                                          //3 读档成功 
	mgr.DelOpt(id);                                                     //4 读档失败调用

	                                                                    //5 加入操作请求
	auto opt = [&](TargetType &target){};                              //注意，回调函数，引用了对象，自己就负责保证对象不野，不然就未定义结果          
	mgr.AddOpt(target_id, opt);

	或者

	auto opt = std::bind(bindFun, _1, ...);
	mgr.AddOpt(target_id, opt);

	或者 

	auto opt = std::bind(&TargetType::Fun, _1, ...);//指定成员函数
	mgr.AddOpt(target_id, opt);

*/

#pragma once
#include "../typedef.h"
#include <vector>
#include <map>
#include <utility>
#include <functional>


#define L_DEBUG(x, ...)  printf(x, ##__VA_ARGS__); //根据自己项目自定义日志

//@Obj			 被操作的对象
//@ObjId		 对象的 id
//@MAX_OPT_NUM   每个对象最大缓存函数对象数
template<class Obj, class ObjId=uint64, const uint32 MAX_OPT_NUM = 3>
class  DelayOptMgr
{
public:
	using OptFun = std::function<void(Obj &)>;
	using OptFunFail = std::function<void(ObjId id)>;
private:
	struct Fun 
	{
		OptFun fun=nullptr;
		OptFunFail funFail = nullptr;
	};
	typedef std::vector<Fun> VecDelayOpt;
	typedef std::map<ObjId, VecDelayOpt> Id2Vec;

	bool m_is_opting = false;				                            //true表示进入BaseDelayOptMgr::OptTarget运行中
	Id2Vec m_id_2_vec;		                                            //id 2 vec, vec ==opt list
public:
	//加一个操作.		(目标找到马上执行，不在就等调用  HandleTarget 再操作)
	//注意：你传一些指针，引用进去延后调用，就需要注意是否会野指针。
	//@optFail 获取Obj失败的情况调用
	void AddOpt(ObjId target_id, OptFun opt, OptFunFail optFail = nullptr);
	void OptTarget(ObjId target_id, Obj &target);//对目标操作缓存操作
	void DelOpt(ObjId target_id);//调用删除目标缓存操作  (读档失败，目标不存时调用)

	 //for test use
	uint32 GetMaxNum() const {return MAX_OPT_NUM;}
	uint32 GetOptNum(ObjId target_id);
private:
    virtual Obj *OnFindTarget(ObjId target_id)=0;			//实现查找内存目标
	virtual void OnMissTarget(ObjId target_id) = 0;				    //实现，请求读档。  (内存没有对象时会调用）


public:

};


template<class Obj, class ObjId, const uint32 MAX_OPT_NUM>
void DelayOptMgr<Obj, ObjId, MAX_OPT_NUM>::OptTarget(ObjId target_id, Obj &target)
{
	if (m_is_opting)
	{
		return;//跑这里属于递归调用，退出不用处理。第一次调用进来的函数会处理
	}
	m_is_opting = true;
	int cnt = 0;
	while (true)
	{
		//////////////测试错误代码，后期没错可以删掉//////////
		cnt++;
		if (cnt > 100)
		{
			L_DEBUG("error,  endless loop?\n");
		}
		////////////////////////////////////////
		auto it = m_id_2_vec.find(target_id);
		if (it == m_id_2_vec.end())
		{
			m_is_opting = false;
			return;
		}
		//复制一份,避免直接引用 m_id_2_vec 导致下面递归错误。 用局部变量 vec_opt 操作，确保不会被回调函数修改。
		VecDelayOpt vec_opt;
		vec_opt.swap(it->second);
		m_id_2_vec.erase(it);

		for(Fun opt: vec_opt)
		{
			if (nullptr == opt.fun)
			{
				L_DEBUG("error, why save null point?\n");
				continue;
			}
			opt.fun(target);//里面会可能继续调用AddOpt，给m_id_2_vec[id]加成员
		}
	}
	m_is_opting = false;
}

template<class Obj, class ObjId, const uint32 MAX_OPT_NUM>
void DelayOptMgr<Obj, ObjId, MAX_OPT_NUM>::AddOpt(ObjId target_id, OptFun opt, OptFunFail optFail )
{
	if (nullptr == opt || 0 == target_id)
	{
		L_DEBUG("error para \n");
		return;
	}

	VecDelayOpt &vec_opt = m_id_2_vec[target_id];
	if (vec_opt.size() >= MAX_OPT_NUM)
	{
		//缓存操作太多了		
		L_DEBUG("error, req is too more, id=%ld\n", target_id);
		return;
	}

	vec_opt.push_back({ opt, optFail });

	if (Obj *pTarget = OnFindTarget(target_id))
	{
		OptTarget(target_id, *pTarget);
	}
	else
	{
		//req get target from db
		if (vec_opt.size() == 1) //第一个操作才请求读库
		{
			OnMissTarget(target_id);
		}
	}
}


template<class Obj, class ObjId, const uint32 MAX_OPT_NUM>
void DelayOptMgr<Obj, ObjId, MAX_OPT_NUM>::DelOpt(ObjId target_id)
{
	typename Id2Vec::iterator it = m_id_2_vec.find(target_id);
	if (it == m_id_2_vec.end())
	{
		return;
	}
	VecDelayOpt& vec_opt = it->second;
	for (Fun &v : vec_opt)
	{
		if (v.funFail != nullptr)
		{
			v.funFail(target_id);
		}
	}
	m_id_2_vec.erase(it);
}

template<class Obj, class ObjId, const uint32 MAX_OPT_NUM>
uint32 DelayOptMgr<Obj, ObjId, MAX_OPT_NUM>::GetOptNum(ObjId target_id)
{
	typename Id2Vec::iterator it = m_id_2_vec.find(target_id);
	if (it == m_id_2_vec.end())
	{
		return 0;
	}
	VecDelayOpt &vec_opt = it->second;
	return vec_opt.size();
}
