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

	//һ��ϵͳ�Զ���ѭ�� timer�� ��ʱ���øú�����  ��ʱ30MS���Ұɣ����������ܺ;�ȷ��ѡ��
	void OnTimer();
private:

};




//end file
