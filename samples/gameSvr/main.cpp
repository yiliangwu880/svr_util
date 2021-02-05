/* 

*/

#include <string>
#include "unit_test.h"
#include "single_progress.h"
#include "App.h"
#include "PlayerMgr.h"
#include "game_util/publish_subscribe.h"



using namespace su;
using namespace std;
int main(int argc, char* argv[])
{
	TriggerEvent<AE_CFG_INI>(1);
	TriggerEvent<AE_INI_NET>(1);
	PlayerMgr::Ins().Login();


	UNIT_ASSERT(CfgMgr::Ins().m_isInit);
	UNIT_ASSERT(CfgMgr::Ins().m_isNetInit);
	printf("---end----\n");
	while (1)
	{
	}
	return 0;
}

