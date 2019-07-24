/*
author: YiliangWu
ʵ��cd���
*/

#pragma once

#include <assert.h>
#include <time.h>
#include "typedef.h"
#include "cnt_typedef.h"

namespace su
{
/*
brief: ����ʱ����
criteria:
) ��ʼ������״̬,����startCD����cd״̬
) ʱ�����Ź�CD���ں󣬻��������״̬
*/
class CdDefault
{
public:
	//param: start	cd��ʼʱ��
	explicit CdDefault(const time_t& period, time_t start=0);
	bool IsCD() const;
	void StartCD();
	void ClearCD();
	time_t GetCountDownTime() const;
	//�ı�CD���ڣ���ǰһCD��ȴ��û����isCD����µ��ã�������CD��
	void ChangePeriod(const time_t& period);

	time_t GetStart() const {return m_start;}
	time_t GetPeriod() const {return m_period;}

private:
	time_t m_start;		
	time_t m_period;	
};

/*
brief:�ۻ�����ʱ��ָ����ֵ������ǿ����ȴ״̬
����:
)��ʼ��Ϊ����״̬��ÿ�ε���accumulate �����ۻ�����ʱ�䡣
)����ʱ���������ۻ�ʱ��ʱ�����ǿ����ȴ״̬
)ǿ����ȴ״̬ʱ��������ʱ��0�ű������״̬��
*/
template<typename Tick>
class AccumulCDBase
{
public:
	AccumulCDBase(const Tick &forceCdDist)
		:m_endCdTick(Tick())
		,m_isForceCd(false)
		,m_forceCdDist(forceCdDist)
	{
	}
	virtual ~AccumulCDBase(){};

	bool IsForceCD() 
	{	
		if (CurrentTick()>=m_endCdTick)
		{
			m_isForceCd = false;
		}
		return m_isForceCd;
	}

	Tick AddCD(Tick accumTick)
	{
		const Tick current = CurrentTick();
		if (current>=m_endCdTick)
		{
			m_endCdTick = current+accumTick;
		}
		else
		{
			m_endCdTick += accumTick;
		}
		Tick accum=m_endCdTick-current;
		if (accum>=m_forceCdDist)
		{
			m_isForceCd = true;
		}
		return accum;
	}

	inline void ClearCD()
	{
		m_endCdTick=Tick();
		m_isForceCd = false;
	}
	
	inline Tick GetCD()
	{
		const Tick current = CurrentTick();
		if (current>=m_endCdTick)
		{
			return 0;
		}			
		return m_endCdTick-current;
	}
	virtual Tick CurrentTick() const =0;

protected://don't access member data unless serialize control
	Tick m_endCdTick;			// cd ending tick
	bool m_isForceCd;
private:		
	Tick m_forceCdDist;			// force cd tick distance
};

/*
brief:����ڼ������ڣ��������ڳ���Ϊ�̶������͡������ܣ��죬Сʱ������������Ϊ�꣬�¡�
����: 
)period must be one of (week,day,hours,minue,half hours,half day)
)limit<period ���ڿ�ʼ�����ʱ�䣨�ֽ�㣩
)�ӱ���ʱ��1970-01-04 00:00:00������ Ϊ����ֵ0��ʼ����,��ǰ������һ��������ֵ0��ʾ

���磺 һ������Ϊ���ڣ� 24*3600*2���ܶ�0ʱ�� Ϊ�ֽ�㣬ĳx�ܵ�����һ��ʾ��y�����ڡ� ��ôx�ܵ����ڶ���ʾy+1�����ڡ�
ʹ�����ӣ�
	CurCycleNum obj(3600*24, 3600);  //����Ϊ�գ��ֽ��Ϊ1��
	uint32 last_num = 0;
	while(1)
	{
		uint32 cur_num = obj.GetCurrent();  ����1970-xxx�����ڵ�������
		if (last_num != cur_num)
		{
			last_num = cur_num
			//to do ÿ������
		}
	}
	
*/
class CurCycleNum
{
public:
	//para period :����
    //para limit�����ڿ�ʼ�����ʱ��. �����ڷֽ��ʱ��Ϊ����x, ���ڷֽ��ʱ��-1������Ϊx-1��
	explicit CurCycleNum(const time_t &period, const time_t &limit = 0);
    //����ָ��ʱ����������� 
	//parameter: const time_t &CurrentTick, ʱ���
	time_t Get(const time_t &cur_time) const;
	//���ص�ǰʱ���������
	time_t GetCurrent() const;

	//����ĳ��ʼʱ�䵽Ŀǰ����������
	static time_t GetCurPeriodNum(time_t start, time_t period);

public:
	static const time_t START_SEC = 3*24*3600-8*3600;	//�ӱ���ʱ��1970-01-04 00:00:00������ Ϊ����ֵ0��ʼ����,��ǰ������һ��������ֵ0��ʾ
	// static const time_t START_SEC = 3*24*3600;   //������ʱ���������������

private:
    time_t m_period;	//����ʱ�䳤�ȡ�
	time_t m_start;		//��һ�����ڿ�ʼʱ�䡣	
};


//brief:
//����CurCycleNum�� �����ڣ��������ʱ��㡣
//���򣺺������ڳ���Ϊ�̶������͡������ܣ��죬Сʱ������������Ϊ�꣬�¡�
//
//���磺ÿ��һ���ܶ� FB ���á� 
//  VecInt64 vec_limit;
//	vec_limit.push_back(3600 * 24 * 1);//��һ
//	vec_limit.push_back(3600 * 24 * 2);//�ܶ�
//	CycleMoreReset obj(3600 * 24 * 7, vec_limit);
//0�� ��1�� ������Ϊ1
//0�� ��2�� ������Ϊ2
//1�� ��1�� ������Ϊ3
//1�� ��2�� ������Ϊ4
//������

class CycleMoreReset
{
public:
	//para period ����
	//para vec_limit �������ٷָ��ʱ��㡣 ƫ��ֵ
	explicit CycleMoreReset(const time_t &period, const VecInt64 &vec_limit);
	//���ص�ǰʱ���������
	time_t GetCurCycleNum() const;

private:
	VecInt64 m_vec_limit;
	time_t m_period;	//����ʱ�䳤�ȡ�
};

/*
brief:�������ڼ��㣬���Ի�ȡ��ʷ��ȡ��Ŀǰ���ŵ��������� (ûʲô���ü�ֵ��ֱ���ù� CurCycleNum �����)
���� 
	���󴴽�����¼��ǰ������Ϊ�����ʷ��������
	����getCount(),��ȡ����ʷ��������ʼ��Ŀǰ���ŵ�������
 */ 
class PeriodCnt
{
public:
	PeriodCnt(const time_t &period, const time_t &limit);
	//��ȡ����һ�ε��øú��������ε������ŵ�������
	time_t getCnt();

	//Ԥ��������ŵ�������
	time_t previewCnt() const;

	time_t Save(){return m_last;}
	void Read(time_t last){m_last = last;}
private:
	time_t m_last; //�����ȡ����������
	CurCycleNum m_current_period_num;
};


/*
brief:�������� ʱ����������������ֵ,������Ϸ��ÿ���Ӽ�1������buf,debuf
rule:
	1 Period Ϊ��������ÿ������ֵ��1�� m_num ��1
	2 m_num �����������ֻ�ܵ�m_maxNum��
	3 ����addExtNum ���Զ�������m_num��û���ޡ�
*/

struct PeriodAccumulate
{
public:
    //para: const time_t &period, ����
    //para: uint32 max_num, ���ڻظ���ֵ���ֵ
	PeriodAccumulate(const time_t &period, uint32 max_num);

	uint32 currentNum();

	void costNum(uint32 cost);

	void addExtNum(uint32 add);

	void setMaxNum(uint32 max_num);

private:
	uint32 m_num;
	uint32 m_max_num;
	time_t m_last_cycle_cnt;  //�������������
	CurCycleNum m_period_cnt;	
};
}