/*
author: YiliangWu
杂项：
)id生成器
)score guard

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include <time.h>
#include "typedef.h"
#include <functional>

#define UNUSED(x) (void)(x)  //无用代码，用来去编译警告

namespace su
{

//唯一id  高32用timestamp, 低32用自增。  1秒内，产生少于4亿，就能保证唯一
//一般几行代码，重复跑4亿次，都要十几秒了。 所以实际情况不会重复
//32位表示时间戳，够用一百多年不重复。
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


} //end namespace su


//end file
