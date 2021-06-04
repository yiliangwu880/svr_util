/* 

*/

#include <string>
#include "unit_test.h"
#include "single_progress.h"

using namespace su;
using namespace std;

int main(int argc, char* argv[])
{

	//UnitTestMgr::Ins().Start();
	UnitTestMgr::Ins().Run("event_mgr");
	return 0;
}

