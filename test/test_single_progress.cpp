
//#include "my_socket.h"
#include "single_progress.h"
#include "unit_test.h"

UNITTEST(test_single_progresss)
{
	UNIT_INFO("test_single_progresss");
	SingleProgress::Obj().CheckSingle("my_name");

}