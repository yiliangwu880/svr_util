/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/CppUtility.git
brief: ��ʱ�����߼��� 
����:
Ϊ�˱��⴦�����ߺ�������ҵ���ͬ����д��ͬ�������̡�ͳһ��ڴ���
���ӣ�
	class MyOptMgr: public DelayOptMgr<MyTarget, uint64>
	{
	public:
		virtual MyTarget *OnFindTarget(uint64 target_id)		//ʵ�ֲ����ڴ�Ŀ��	
		{
			...
		}

		virtual void OnMissTarget(uint64 target_id)			//ʵ�֣����������  (����û�ж���ʱ����ã�
		{
			...
		}
	};

	MyOptMgr mgr;

	mgr.OptTarget(id, target);                                          //3 �����ɹ� 
	mgr.DelOpt(id);                                                     //4 ����ʧ�ܵ���

	                                                                    //5 �����������
	auto opt = [&](TargetType &target){};                                          
	mgr.AddOpt(target_id, opt);

	����

	auto opt = std::bind(bindFun, _1, ...);
	mgr.AddOpt(target_id, opt);

	���� 

	auto opt = std::bind(&TargetType::Fun, _1, ...);//ָ����Ա����
	mgr.AddOpt(target_id, opt);

*/

#pragma once
#include "../typedef.h"
#include <vector>
#include <map>
#include <utility>
#include <functional>

                                                                        //����Ŀ��

//@Obj			 �������Ķ���
//@ObjId		 ����� id
//@MAX_OPT_NUM   ÿ��������󻺴溯��������
template<class Obj, class ObjId=uint64, const uint32 MAX_OPT_NUM = 3>
class  DelayOptMgr
{
	using OptFun = std::function<void(Obj &)>;
	typedef std::vector<OptFun> VecDelayOpt;
	typedef std::map<ObjId, VecDelayOpt> Id2Vec;

	bool m_is_opting = false;				                            //true��ʾ����BaseDelayOptMgr::OptTarget������
	Id2Vec m_id_2_vec;		                                            //id 2 vec, vec ==opt list
public:
	//��һ������.		(Ŀ���ҵ�����ִ�У����ھ͵ȵ���  HandleTarget �ٲ���)
	//ע�⣺�㴫һЩָ�룬���ý�ȥ�Ӻ���ã�����Ҫע���Ƿ��Ұָ�롣
	void AddOpt(ObjId target_id, OptFun opt);	

	                                                                    //��Ŀ������������
	void OptTarget(ObjId target_id, Obj &target);

	void DelOpt(ObjId target_id);								        //����ɾ��Ŀ�껺�����  (����ʧ�ܣ�Ŀ�겻��ʱ����)

	 //for test use
	uint32 GetMaxNum() const {return MAX_OPT_NUM;}
	uint32 GetOptNum(ObjId target_id);
private:
    virtual Obj *OnFindTarget(ObjId target_id)=0;			//ʵ�ֲ����ڴ�Ŀ��
	virtual void OnMissTarget(ObjId target_id) = 0;				    //ʵ�֣����������  (����û�ж���ʱ����ã�


public:

};


template<class Obj, class ObjId, const uint32 MAX_OPT_NUM>
void DelayOptMgr<Obj, ObjId, MAX_OPT_NUM>::OptTarget(ObjId target_id, Obj &target)
{
	if (m_is_opting)
	{
		return;//���������ڵݹ���ã��˳����ô�����һ�ε��ý����ĺ����ᴦ��
	}
	int cnt = 0;
	while (true)
	{
		m_is_opting = true;
		//////////////���Դ�����룬����û�����ɾ��//////////
		cnt++;
		if (cnt > 100)
		{
			printf("error,  endless loop?\n");
		}
		////////////////////////////////////////
		auto it = m_id_2_vec.find(target_id);
		if (it == m_id_2_vec.end())
		{
			m_is_opting = false;
			return;
		}
		//����һ��,����ֱ������ m_id_2_vec ��������ݹ����
		VecDelayOpt vec_opt;
		vec_opt.swap(it->second);
		m_id_2_vec.erase(it);

		for(OptFun opt: vec_opt)
		{
			if (nullptr == opt)
			{
				printf("error, why save null point?\n");
				continue;
			}
			opt(target);//�������ܼ�������AddOpt����m_id_2_vec[id]�ӳ�Ա
		}
	}
	m_is_opting = false;
}

template<class Obj, class ObjId, const uint32 MAX_OPT_NUM>
void DelayOptMgr<Obj, ObjId, MAX_OPT_NUM>::AddOpt(ObjId target_id, OptFun opt)
{
	if (nullptr == opt || 0 == target_id)
	{
		printf("error para \n");
		return;
	}

	VecDelayOpt &vec_opt = m_id_2_vec[target_id];
	if (vec_opt.size() >= MAX_OPT_NUM)
	{
		//�������̫����		
		printf("error, req is too more, id=%lld\n", target_id);
		return;
	}

	vec_opt.push_back(opt);

	if (Obj *pTarget = OnFindTarget(target_id))
	{
		OptTarget(target_id, *pTarget);
	}
	else
	{
		//req get target from db
		if (vec_opt.size() == 1) //��һ���������������
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
