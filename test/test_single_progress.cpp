
#include "su_include.h"
#include "single_progress.h"
#include "unit_test.h"

namespace{
	void MyExitProccessCB()
	{

		UNIT_INFO("MyExitProccessCB");
	}

	void testStart()
	{
		UNIT_INFO("testStart");
		SPMgr::Obj().Start("test_single_pc", MyExitProccessCB);

	}
}
UNITTEST(test_single_progresss)
{
	//SingleProgress::Obj().CheckSingle("my_name");
	testStart();
}