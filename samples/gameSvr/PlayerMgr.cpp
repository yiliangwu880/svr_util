/* 

*/
#include <string>
#include "unit_test.h"
#include "singleton.h"
#include "PlayerMgr.h"
#include "static_reg.h"

using namespace su;
using namespace std;

void ON_AE_CFG_INI( int num)
{
	CfgMgr::Obj().Init();
	printf("ON_AE_CFG_INI\n");
}

STATIC_RUN(RegEvent<AE_CFG_INI>(ON_AE_CFG_INI);)

void ON_AE_INI_NET(int num)
{
	CfgMgr::Obj().m_isNetInit = true;
}

STATIC_RUN(RegEvent<AE_INI_NET>(ON_AE_INI_NET);)

void ON_PE_LOGIN(Player &player)
{
	UNIT_ASSERT(player.n == 33);
	printf("ON_PE_LOGIN %d\n", player.n);
}
STATIC_RUN(RegEvent<PE_LOGIN>(ON_PE_LOGIN);)



void PlayerMgr::Login()
{
	m_player.n = 33;
	TriggerEvent<PE_LOGIN>(m_player);
}
