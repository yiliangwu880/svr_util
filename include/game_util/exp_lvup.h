/************************************************************************/
/*
brief: ���ľ�������
author:YiliangWu
Сϵͳ�������٣�һ��д����BUG�������Ѷȵġ���ɸ��õİ�
*/
/************************************************************************/

#pragma once

#include "typedef.h"

namespace su
{
class LvupBase
{
	uint32 m_lv = 0;
	uint64 m_exp = 0;   //���飬���������ĵ�һ��������

public:
	void AddExp(uint64 addNum);
	void AddExpNew(uint64 addNum);
	uint32 GetLv() const { return m_lv; }
	uint64 GetExp() const { return m_exp; }
	void SetLv(uint32 lv);
	void SetExp(uint64 exp);
	//��ȡ��������һ����Ҫ����exp
	//@next_lv ��һ��
	virtual uint64 GetNextLvNeedExp(uint32 next_lv) const =0;
	virtual uint32 GetMaxLv() const =0;

private:
	//ÿ��һ���ص�
	virtual void OnLvup()=0;
};
}
