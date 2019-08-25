/*
author: YiliangWu

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "singleton.h"
#include "typedef.h"
#include "cnt_typedef.h"


class SuMgr : public Singleton<SuMgr>
{
public:
	bool Init();

	//一般系统自定义循环 timer， 定时调用该函数。  定时30MS到1sec都行，看具体性能和精确度选择。
	void OnTimer();
private:

};




//end file
