#include "su_mgr.h"
#include "time/su_timer.h"
#include "time/su_timestamp.h"
#include "BacktraceInfo.h"

using namespace su;

bool SuMgr::Init()
{
	CBacktraceInfo::Obj().RegHangUpHandler();
	return true;
}

void SuMgr::OnTimer()
{
	SysTime::Obj().Refresh();
	TimeDriver::Obj().CheckTimeOut();
}
