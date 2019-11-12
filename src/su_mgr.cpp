#include "su_mgr.h"
#include "time/su_timer.h"
#include "time/su_timestamp.h"
#include "BacktraceInfo.h"
#include "stdlib.h"

using namespace su;

bool SuMgr::Init()
{
	srand((unsigned int)time(0));
	CBacktraceInfo::Obj().RegHangUpHandler();
	return true;
}

void SuMgr::OnTimer()
{
	SysTime::Obj().Refresh();
	TimeDriver::Obj().CheckTimeOut();
}
