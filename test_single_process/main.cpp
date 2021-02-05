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

	SingleProgress::Ins().Check(argc, argv, "single_process_test");


	while (true)
	{
		if (SingleProgress::Ins().IsExit())
		{
			printf("stop");
			break;
		}
	}
	//UnitTestMgr::Ins().Start();
	return 0;
}

