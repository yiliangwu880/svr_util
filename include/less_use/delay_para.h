/*
最新观点： 等研究如何废弃重新设计。用 函数对象绑定延时参数，将来调用更好！
还没完善!!!
参考用吧：一般代码设计成简单点。 不需要用这种，不然有点复杂，可阅读性低。 
设计简单点比如：player 发送消息前，参数保存在player对象里面，等回调的时候再通过playerid去取完事。

brief: 延时参数管理， 用户动态调用创建初始化， 延时（收到网络消息后），获取参数，自动释放。
例子：
	网络请求例子
	//网络请求是，构造动态参数
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
	send(p->id); --发送到网络，等回调


	//网络回调时调用
	void onRev(uint64 id)
	{
		MyPara Get(id); //自动释放 p内存
		assert(guard.m_para != NULL);
		assert(guard.m_para->a == 1);
	}


	 //超时清理参数
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
		uint64 create_tm = 0; //创建时时间戳
		uint64 id = 0;
		MyData d;
	};
	typedef std::map<uint64, DelayData> Id2DalayData;

	static const uint64 TIME_OUT_SEC = 60 * 5;  //过期时间，秒. 时间可以长点，避免动态内存重复利用产生BUG。 回调参数获取的内存不是自己的。
	Id2DalayData m_id_2_data; //参数 2 创建时间戳
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

	//失败返回nullptr
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

	//建议5秒调用一次
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
	//一般不设置，用默认的就行
	void SetTimeOutSec(uint64 sec)
	{
		m_time_out_sec = sec;
	}


private:
	DelayParaMgr() {};

};
