
#include "game_util/game_cheat.h"
#include "unit_test.h"


using namespace std;
using namespace su;

namespace
{
	void test1()
	{
	

		string s = GameCheat::Ins().DoCmd(NULL, "doHandle1 abc,:ccd 123,321");
		//UNIT_INFO("para list: %s", s.c_str());
		UNIT_ASSERT(s == " abc ccd 123 321");
		s = GameCheat::Ins().DoCmd("doHandle1 abc,:ccd 123,321");
		UNIT_ASSERT(s == " abc ccd 123 321");

		s = GameCheat::Ins().DoCmd("MyClass_doHandle1 abc,:ccd 123,321");
		//UNIT_INFO("result=%s", s.c_str());
		UNIT_ASSERT(s == "static");


	}


	
	REG_PLAYER_DO_CMD(doHandle1);
	std::string doHandle1(GamePlayer *pPlayer, const VecStr &vecStr)
	{
		string s;
		for(auto &v: vecStr)
		{
			s += " ";
			s += v;
		}
		return s;
	}

	REG_DO_CMD(doHandle1);
	std::string doHandle1(const VecStr &vecStr)
	{
		string s;
		for (auto &v : vecStr)
		{
			s += " ";
			s += v;
		}
		return s;
	}

	bool g_1 = false;
	class MyClass
	{
	public:
		static std::string doHandle1(const VecStr &vecStr)
		{
			g_1 = true;
			return "static";
		}
	};
	REG_DO_CMD_EX(MyClass_doHandle1, MyClass::doHandle1)

}//end namespace

UNITTEST( test_game_cheat)
{
	test1();
	UNIT_ASSERT(g_1);
}