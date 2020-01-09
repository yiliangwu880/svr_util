/*
brief: ¾²Ì¬×¢²áÊ¹ÓÃÀý×Ó
*/

#include "game_util/exp_lvup.h"
#include "unit_test.h"


using namespace std;
using namespace su;

namespace
{
	class MyLv : public LvupBase
	{
	public:
		uint32 m_on_lv_cnt=0; 
		virtual uint64 GetNextLvNeedExp(uint32 next_lv) const override;
		virtual uint32 GetMaxLv() const override;

	private:
		virtual void OnLvup() override;
	};

	uint64 MyLv::GetNextLvNeedExp(uint32 next_lv) const
	{
		return 10;
	}

	uint32 MyLv::GetMaxLv() const
	{
		return 10;
	}

	void MyLv::OnLvup()
	{
		m_on_lv_cnt++;
	}

	MyLv g_lv;
}
UNITTEST(exp_lvup)
{

	g_lv.AddExp(10);
	UNIT_ASSERT(g_lv.m_on_lv_cnt == 1);
	g_lv.AddExp(5);
	UNIT_ASSERT(g_lv.m_on_lv_cnt == 1);
	g_lv.AddExp(5);
	UNIT_ASSERT(g_lv.m_on_lv_cnt == 2);
	UNIT_ASSERT(g_lv.GetLv() == 2);
	UNIT_ASSERT(g_lv.GetExp() == 0);

	//test set exp
	g_lv.SetExp(0);
	UNIT_ASSERT(g_lv.GetLv() == 2);
	UNIT_ASSERT(g_lv.GetExp() == 0);


	g_lv.SetExp(80);
	UNIT_ASSERT(g_lv.m_on_lv_cnt == 10);
	UNIT_ASSERT(g_lv.GetLv() == 10);
	UNIT_ASSERT(g_lv.GetExp() == 0);

	//test full lv
	g_lv.AddExp(500);
	UNIT_ASSERT(g_lv.m_on_lv_cnt == 10);
	UNIT_ASSERT(g_lv.GetLv() == 10);
	UNIT_ASSERT(g_lv.GetExp() == 0);

	g_lv.SetExp(500);
	UNIT_ASSERT(g_lv.m_on_lv_cnt == 10);
	UNIT_ASSERT(g_lv.GetLv() == 10);
	UNIT_ASSERT(g_lv.GetExp() == 500);


	//test 

}