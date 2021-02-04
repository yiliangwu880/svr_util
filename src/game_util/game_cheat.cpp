/*
brief:游戏作弊码

*/



#include "game_util/game_cheat.h"
#include "str_util.h"
#include "misc.h"


using namespace std;
using namespace su;

std::string GameCheat::DoCmd( GamePlayer *pPlayer, const char *pChar )
{
	VecStr vec_str;
	VecStr vec_split;
	vec_split.push_back(";");
	vec_split.push_back(",");
	vec_split.push_back(".");
	vec_split.push_back(" ");
	vec_split.push_back(":");
	StrUtil::split(pChar, vec_split, vec_str );
	if (vec_str.empty())
	{
		return "no string";
	}
	StrMapCheatFun::const_iterator it = StrMapCheatFun::Obj().find(vec_str.front());
	if (it==StrMapCheatFun::Obj().end())
	{
		return "can't find handle function";
	}
	vec_str.erase(vec_str.begin());
	DoPlayerCmdFun p_fun = it->second;
	return (*p_fun)(pPlayer, vec_str);
}

std::string GameCheat::DoCmd(const char *pChar)
{
	VecStr vec_str;
	VecStr vec_split;
	vec_split.push_back(";");
	vec_split.push_back(",");
	vec_split.push_back(".");
	vec_split.push_back(" ");
	vec_split.push_back(":");
	StrUtil::split(pChar, vec_split, vec_str);
	if (vec_str.empty())
	{
		return "no string";
	}
	auto it = StrMapCheatFun_DoCmdFun::Obj().find(vec_str.front());
	if (it == StrMapCheatFun_DoCmdFun::Obj().end())
	{
		return "can't find handle function";
	}
	vec_str.erase(vec_str.begin());
	DoCmdFun p_fun = it->second;
	return (*p_fun)(vec_str);
}

