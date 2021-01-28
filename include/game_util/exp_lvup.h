/************************************************************************/
/*
brief: 消耗经验升级
author:YiliangWu
小系统，功能少，一步写对无BUG还是有难度的。搞可复用的吧
*/
/************************************************************************/

#pragma once

#include "typedef.h"

namespace su
{
class LvupBase
{
	uint32 m_lv = 0;
	uint64 m_exp = 0;   //经验，升级会消耗掉一定数量。

public:
	void AddExp(uint64 addNum);
	void AddExpNew(uint64 addNum);
	uint32 GetLv() const { return m_lv; }
	uint64 GetExp() const { return m_exp; }
	void SetLv(uint32 lv);
	void SetExp(uint64 exp);
	//获取升级到下一级需要消耗exp
	//@next_lv 下一级
	virtual uint64 GetNextLvNeedExp(uint32 next_lv) const =0;
	virtual uint32 GetMaxLv() const =0;

private:
	//每升一级回调
	virtual void OnLvup()=0;
};
}
