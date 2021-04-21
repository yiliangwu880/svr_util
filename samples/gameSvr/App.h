/* 

*/
#pragma once
#include <string>
#include "unit_test.h"
#include "singleton.h"
#include "game_util/publish_subscribe.h"

using namespace su;
using namespace std;

//100��ʼ
enum  APP_EVENT 
{
	AE_CFG_INI = 1, //��ʼ������
	AE_AFTER_CFG_INI = 2, //��ʼ�����ú�
	AE_INI_NET = 3, //��ʼ������

};

namespace su
{
	template<>
	struct EventTraits<AE_CFG_INI> {
		using Fun = void(*)(int i);
	};
	template<>
	struct EventTraits<AE_INI_NET> {
		using Fun = void(*)(int i);
	};
}

class CfgMgr : public Singleton<CfgMgr>
{
public:
	bool m_isInit = false;
	bool m_isNetInit = false;
	CfgMgr() {};
	void Init()
	{
		m_isInit = true;
	};
};

