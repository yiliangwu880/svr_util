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
void f2();
int main(int argc, char* argv[])
{
	//init cfg
	CfgMgr::Obj().Init();
	TriggerEvent<AE_CFG_INI>(1);
	//init net
	//loop wait event
	PlayerMgr::Obj().Login();
	printf("---end----\n");
	while (1)
	{
	}
	return 0;
}

