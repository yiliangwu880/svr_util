/*
author: YiliangWu

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "singleton.h"


class SuMgr : public Singleton<SuMgr>
{
public:
	bool Init();

	//һ��ϵͳ�Զ���ѭ�� timer�� ��ʱ���øú�����  ��ʱ30MS���Ұɣ����������ܺ;�ȷ��ѡ��
	void OnTimer();
private:

};




//end file
