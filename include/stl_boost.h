/*
author: YiliangWu
支援标准库，使用更简化
*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>


namespace su
{


//////////////////////////////////////vector////////////////////////////////////
	template <typename Vec>
	inline bool VecFind(const Vec& vec, typename Vec::value_type val)
	{
		typename Vec::const_iterator it = std::find(vec.begin(), vec.end(), val);
		if (it == vec.end())
		{
			return false;
		}
		return true;
	}

	//vector fast remove an element. but change the remain elements order.
	template <typename Vec>
	inline bool VecRemove(Vec& vec, typename Vec::value_type val)
	{
		for(typename Vec::iterator iter = vec.begin() ; iter != vec.end(); ++iter)
		{
			if(*iter == val)
			{
				*iter = vec.back();
				vec.erase(vec.end()-1);
				return true;
			}
		}
		return false;
	}

	template <typename Vec>
	inline void VecRemoveByIndex(Vec& vec, typename Vec::size_type index)
	{
		vec[index] = vec[vec.size()-1];
		vec.erase(vec.end()-1);
	}

	template <typename Vec>
	inline void VecRemove(Vec& vec,typename Vec::iterator it)
	{
		*it = vec.back();
		vec.erase(vec.end()-1);
	}

	template <typename Vec>
	void VecUnique(Vec &words)
	{
		std::sort(words.begin(), words.end());
		typename Vec::iterator end_unique = unique(words.begin(), words.end());
		words.erase(end_unique, words.end());
	}
	template <typename Vec, typename AddCtn>
	void VecAppend(Vec &vec, const AddCtn &add)
	{
		vec.insert(vec.end(), add.begin(), add.end());
	}
	

	////////////////////////////////easy use map//////////////////////////////////////////
	template <typename Map>
	inline typename Map::mapped_type *MapFind(Map &map, typename Map::key_type key)
	{
		auto iter = map.find(key);
		if (iter==map.end())
		{
			return NULL;
		}		
		return &(iter->second);	
	}

	template <typename Map>
	inline  const typename Map::mapped_type *MapFind(const Map &map, typename Map::key_type key)
	{
		typename Map::const_iterator iter = map.find(key);
		if (iter == map.end())
		{
			return NULL;
		}
		return &(iter->second);
	}

	template <typename Map>
	inline   bool MapErase(Map &map, typename Map::key_type key)
	{
		return 1 == map.erase(key);
	}

	template <typename Map>
	inline std::pair<typename Map::iterator, bool> MapInsert(Map &map, typename Map::key_type key, typename Map::mapped_type v)
	{
		return  map.insert(std::make_pair(key, v));
	}


//////////////////////////////////////////////////////////////////////////

} //end namespace YiLiang


//end file