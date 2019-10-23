#include "su_math.h"


namespace su
{
	void combine(uint32 total_num, int combine_num, std::vector<std::vector<int>> &all_result, int index = 0, int deep = 0, bool is_start)
	{
		static std::vector<int> result;
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
			std::vector<int> t;
			for (int i = 0; i < combine_num; i++)
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