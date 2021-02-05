/* 

*/

#include <string>
#include "unit_test.h"
#include "single_progress.h"
#include <unistd.h>

using namespace su;
using namespace std;



int main(int argc, char* argv[])
{

	SingleProgress::Obj().Check(argc, argv, "single_process_test");


	while (true)
	{
		if (SingleProgress::Obj().IsExit())
		{
			printf("stop");
			break;
		}
	}
	//UnitTestMgr::Obj().Start();
	return 0;
}

