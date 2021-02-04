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
	printf("ON_AE_CFG_INI\n");
}

STATIC_RUN(RegEvent<AE_CFG_INI>(ON_AE_CFG_INI);)


void ON_PE_LOGIN(Player &player)
{
	printf("ON_PE_LOGIN %d\n", player.n);
}

STATIC_RUN(RegEvent<PE_LOGIN>(ON_PE_LOGIN);)


void f2()
{
	RegEvent<AE_CFG_INI>(ON_AE_CFG_INI);
	RegEvent<PE_LOGIN>(ON_PE_LOGIN);
}