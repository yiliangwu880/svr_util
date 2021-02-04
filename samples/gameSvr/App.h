/* 

*/
#pragma once
#include <string>
#include "unit_test.h"
#include "singleton.h"
#include "game_util/publish_subscribe.h"

using namespace su;
using namespace std;

//100��ʼ
enum  APP_EVENT 
{
	AE_CFG_INI = 1,
	AE_AFTER_CFG_INI = 2,
	AE_INI_NET = 3,

};

namespace su
{
	template<>
	struct EventTraits<AE_CFG_INI> {
		using Fun = void(*)(int i);
	};
}

class CfgMgr : public Singleton<CfgMgr>
{
public:
	CfgMgr() {};
	void Init()
	{};
};

