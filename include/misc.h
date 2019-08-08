/*
author: YiliangWu
���
)id������
)get array length
)easy use placement new
)vector fast remove an element. but change the remain elements order.
*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "typedef.h"
#include <time.h>
#define UNUSED(x) (void)(x)

namespace su
{

//Ψһid  ��32��timestamp, ��32��������  1���ڣ���������4�ڣ����ܱ�֤Ψһ
//һ�㼸�д��룬�ظ���4�ڴΣ���Ҫʮ�����ˡ� ����ʵ����������ظ�
//32λ��ʾʱ���������һ�ٶ��겻�ظ���
class IdCreater  
{
public:
	uint64 CreateId()
	{
		time_t sec;
		time(&sec);
		m_seed++;
		uint64 id = sec << 32;
		id = id | m_seed;
		return id;
	}

private:
	uint32 m_seed = 0;
};



} //end namespace YiLiang


//end file
