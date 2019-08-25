/*
brief:
*/
#pragma once
#include "typedef.h"
#include <vector>
#include <set>
#include <map>
#include <string>

namespace su 
{
	//常用容器类型简写
typedef std::map<uint32, uint32>			Uint32_2_Uint32;
typedef std::map<int32, int32>				Int32_2_Int32;
typedef std::map<std::string, std::string>  Str2Str;

typedef std::set<uint32>					SetUint32;
typedef std::set<uint64>					SetUint64;
typedef std::set<std::string>				SetStr;

typedef std::vector<std::string>	VecStr;
typedef std::vector<uint8>			VecUint8;
typedef std::vector<uint16>			VecUint16;
typedef std::vector<int32>			VecInt32;
typedef std::vector<int>			VecInt;
typedef std::vector<uint32>			VecUint32;
typedef std::vector<uint64>			VecUint64;
typedef std::vector<int64>			VecInt64;

}