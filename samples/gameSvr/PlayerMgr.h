/* 

*/
#pragma once
#include <string>
#include "unit_test.h"
#include "singleton.h"
#include "App.h"

using namespace su;
using namespace std;

//500¿ªÊ¼
enum  PLAYER_EVENT
{
	PE_LOGIN = 500,
	PE_LVUP = 501,
};
class Player;
namespace su
{
	template<>
	struct EventTraits<PE_LOGIN> {
		using Fun = void(*)(Player &);
	};
}

class SubSys1
{
public:

};
class SubSys2
{
public:

};
class Player
{
public:
	SubSys1 m_sub1;
	SubSys2 m_sub2;
	int n;
};

class PlayerMgr : public Singleton<PlayerMgr>
{
public:
	PlayerMgr() {};
	Player m_player;
	void Login();
};


