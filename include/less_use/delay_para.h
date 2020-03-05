/*
��û����!!!
�ο��ðɣ�һ�������Ƴɼ򵥵㡣 ����Ҫ�����֣���Ȼ�е㸴�ӣ����Ķ��Ե͡� 
��Ƽ򵥵���磺player ������Ϣǰ������������player�������棬�Ȼص���ʱ����ͨ��playeridȥȡ���¡�

brief: ��ʱ�������� �û���̬���ô�����ʼ���� ��ʱ���յ�������Ϣ�󣩣���ȡ�������Զ��ͷš�
���ӣ�
	������������
	//���������ǣ����춯̬����
	class MyPara
	{
	public:
	MyPara(int p1, int p2)
	{
	a = p1; b = p2;
	}
	int a, b;
	};

	MyPara *p = DelayParaMgr::Instance().Push(1, 2);
	assert(p != NULL);
	send(p->id); --���͵����磬�Ȼص�


	//����ص�ʱ����
	void onRev(uint64 id)
	{
		MyPara Get(id); //�Զ��ͷ� p�ڴ�
		assert(guard.m_para != NULL);
		assert(guard.m_para->a == 1);
	}


	 //��ʱ�������
	 DelayParaMgr::Instance().OnTimer();
	
*/

#pragma once
#include "typedef.h"
#include <vector>
#include <map>
#include <utility>



struct MyData
{
	int i=0;
};

class DelayParaMgr
{
	struct DelayData
	{
		uint64 create_tm = 0; //����ʱʱ���
		uint64 id = 0;
		MyData d;
	};
	typedef std::map<uint64, DelayData> Id2DalayData;

	static const uint64 TIME_OUT_SEC = 60 * 5;  //����ʱ�䣬��. ʱ����Գ��㣬���⶯̬�ڴ��ظ����ò���BUG�� �ص�������ȡ���ڴ治���Լ��ġ�
	Id2DalayData m_id_2_data; //���� 2 ����ʱ���
	uint64 m_id_seed = 0;
	uint64 m_time_out_sec = TIME_OUT_SEC;

public:
	static DelayParaMgr &Instance()
	{
		static DelayParaMgr obj;
		return obj;
	}
	
	MyData &Push()
	{
		m_id_seed++;

		DelayData d;
		d.id = m_id_seed;
		time_t *t = (time_t *)(&d.create_tm);
		time(t);
		return m_id_2_data[d.id].d;
	}

	//ʧ�ܷ���nullptr
	bool Pop(uint64 id, MyData &data)
	{
		auto it = m_id_2_data.find(id);
		if (it == m_id_2_data.end())
		{
			return false;
		}
		data = it->second.d;
		m_id_2_data.erase(it);
		return true;
	}

	//����5�����һ��
	void CheckTimeOut()
	{
		time_t cur = 0;
		time(&cur);
		for (auto it = m_id_2_data.begin(); it != m_id_2_data.end();)
		{
			if (cur - it->second.create_tm > m_time_out_sec)
			{
				it = m_id_2_data.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	uint32_t GetParaNum()
	{
		return m_id_2_data.size();
	}
	//һ�㲻���ã���Ĭ�ϵľ���
	void SetTimeOutSec(uint64 sec)
	{
		m_time_out_sec = sec;
	}


private:
	DelayParaMgr() {};

};
