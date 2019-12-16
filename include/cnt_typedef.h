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

using Str2Str         = std::map<std::string, std::string>	;
using SetUint32       = std::set<uint32>					;
using SetUint64       = std::set<uint64>					;
using SetStr          = std::set<std::string>				;
using VecStr          = std::vector<std::string>			;
using VecUint8        = std::vector<uint8>					;
using VecUint16       = std::vector<uint16>					;
using VecInt32        = std::vector<int32>					;
using VecInt          = std::vector<int>					;
using VecUint32       = std::vector<uint32>					;
using VecUint64       = std::vector<uint64>					;
using VecInt64        = std::vector<int64>					;

}