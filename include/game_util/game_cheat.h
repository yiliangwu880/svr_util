/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/svr_util.git

brief:执行字符串命令， 游戏作弊码,
特点：
	1）新加作弊功能的方法非常方便，而且可以放在一个代码区域： （免去到处翻代码文件添加声明定义）
		定义新函数， 
		 REG_PLAYER_DO_CMD(FuntionName); 
		 std::string FuntionName(GamePlayer *pPlayer, const VecStr &vecStr){...}

	2）命令和函数名一样。 比如 "AddMoney 1000" 就执行 AddMoney函数. (杜绝相同意义东西命名不一样，代码难浏览)

字符串命令用; , . : 空格 分开参数。

缺点：实现原理难理解


sample:
	GameCheat::DoCmd(player, "AddMoney 1000");


	//处理命令"AddMoney 1000"
	REG_PLAYER_DO_CMD(AddMoney);
	std::string AddMoney(GamePlayer *pPlayer, const VecStr &vecStr)
	{
		for(string v: vecStr)
		{
			....
		}
		return "ok";
	}

*/

#pragma once
#include "singleton.h"
#include "typedef.h"
#include "static_reg.h"
#include "cnt_typedef.h"

namespace su
{
	//typedef int GamePlayer;
	class GamePlayer; //玩家类，不同工程名字不一样，手动改吧。
	using DoPlayerCmdFun = std::string(*)(GamePlayer *pPlayer, const VecStr &vecStr);
	using DoCmdFun = std::string(*)(const VecStr &vecStr);
	class GameCheat : public Singleton<GameCheat>
	{
		std::map<std::string, DoPlayerCmdFun> m_str2DoPlayerCmdFun;
		std::map<std::string, DoCmdFun> m_str2DoCmdFun;

	public:
		//执行作弊字符串，返回结果信息。	
		std::string DoCmd(GamePlayer *pPlayer, const char *pChar);
	    std::string DoCmd(const char *pChar);
		void RegPlayer(const char* cmd, DoPlayerCmdFun fun);
		void Reg(const char* cmd, DoCmdFun fun);

	};

#define REG_PLAYER_DO_CMD_EX(cmd, fun)\
	STATIC_RUN(GameCheat::Ins().RegPlayer(#cmd, fun);) 

#define REG_DO_CMD_EX(cmd, fun)\
	STATIC_RUN(GameCheat::Ins().Reg(#cmd, fun);) 

#define REG_PLAYER_DO_CMD(fun)\
std::string fun(GamePlayer *pPlayer, const VecStr &vecStr);\
	STATIC_RUN(GameCheat::Ins().RegPlayer(#fun, fun);) 

#define REG_DO_CMD(fun)\
std::string fun(const VecStr &vecStr);\
	STATIC_RUN(GameCheat::Ins().Reg(#fun, fun);)

}