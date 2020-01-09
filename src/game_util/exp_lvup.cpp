#include "game_util/exp_lvup.h"

namespace su{
void LvupBase::AddExp( uint64 addNum )
{
	uint32 max_lv = GetMaxLv();
	if (m_lv >= max_lv)
	{
		return;
	}
	m_exp+=addNum;

	uint64 needExp = GetNextLvNeedExp(m_lv+1);
	while (m_exp >= needExp)
	{
		m_exp -= needExp;
		m_lv++;
		OnLvup();
		if (m_lv >= max_lv)
		{
			break;
		}
		needExp = GetNextLvNeedExp(m_lv+1);
	};
}

void LvupBase::SetLv(uint32 lv)
{
	uint32 max_lv = GetMaxLv();
	if (lv >= max_lv)
	{
		return;
	}
	m_lv = lv; 
}

void LvupBase::SetExp(uint64 exp)
{
	m_exp = exp;
	AddExp(0);
}
 }