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
	//@total_num 候选总数
	//@combine_num 组合数量
	//@all_result 所有组合结果 
	void combine(uint32 total_num, uint32 combine_num, std::vector<std::vector<uint32>> &all_result, uint32 index = 0, uint32 deep = 0, bool is_start = true);
}


//end file
