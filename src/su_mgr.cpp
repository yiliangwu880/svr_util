#include "su_mgr.h"
#include "time/su_timer.h"
#include "time/su_timestamp.h"
#include "BacktraceInfo.h"
#include "stdlib.h"
#include "su_rand.h"

using namespace su;

bool SuMgr::Init()
{
	//整个svr_util系统 需要初始化的模块都在这里初始化
	Random::Init();
	CBacktraceInfo::Ins().RegHangUpHandler();
	return true;
}

void SuMgr::OnTimer()
{
	SysTime::Ins().Refresh();
	TimeDriver::Ins().CheckTimeOut();
}
