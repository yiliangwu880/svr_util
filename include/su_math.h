/*
数学相关
*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include <time.h>
#include "typedef.h"
#include <functional>

namespace su
{
	//计算组合, 候选数为 0,1,2.....
		//@all_result 所有组合结果 
	void combine(uint32 total_num, int combine_num, std::vector<std::vector<int>> &all_result, int index = 0, int deep = 0, bool is_start = true);
}


//end file
