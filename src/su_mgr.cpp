#include "su_mgr.h"
#include "time/su_timer.h"
#include "time/su_timestamp.h"
#include "BacktraceInfo.h"
#include "stdlib.h"
#include "su_rand.h"

using namespace su;

bool SuMgr::Init()
{
	//����svr_utilϵͳ ��Ҫ��ʼ����ģ�鶼�������ʼ��
	Random::Init();
	CBacktraceInfo::Ins().RegHangUpHandler();
	return true;
}

void SuMgr::OnTimer()
{
	SysTime::Ins().Refresh();
	TimeDriver::Ins().CheckTimeOut();
}
