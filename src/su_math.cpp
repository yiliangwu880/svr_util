#include "su_math.h"


namespace su
{
	void combine(uint32 total_num, uint32 combine_num, std::vector<std::vector<uint32>> &all_result, uint32 index , uint32 deep, bool is_start)
	{
		static std::vector<uint32> result;
		if (is_start == true)
		{
			all_result.clear();
			result.clear();
			result.resize(total_num, 0);
		}
		if (0 == combine_num || 0 == total_num)
		{
			return;
		}
		if (deep >= combine_num)
		{
			std::vector<uint32> t;
			for (uint32 i = 0; i < combine_num; i++)
			{
				t.push_back(result[i]);
			}
			all_result.push_back(t);
			return;
		}
		for (uint32 i = index; i < total_num; i++)
		{
			result[deep] = i;
			combine(total_num, combine_num, all_result, index + 1, deep + 1, false);
			index++;
		}
	}
}