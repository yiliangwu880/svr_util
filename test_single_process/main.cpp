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
	if (0 != daemon(1, 0))
	{
		printf("daemon fail\n");
	}
	SingleProgress::Obj().Check(argc, argv, "test_single_process");

	//���ﲻ�ܵ��� daemon��������Ȼ����Ч�����ܽ��̺����ļ���ϵ�仯��
	//daemon(1, 0)

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

