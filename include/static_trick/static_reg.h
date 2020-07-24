/*
author:yiliangwu
brief: 静态注册使用例子
特点:
	注册可以写在cpp任意地方,免去到处翻代码文件添加声明或者定义
	main函数后才保证初始化成功。

缺点：实现原理不好理解

use example:
//in h file
REG_MAP_NAME_DECLARE(MapName2, string, int)

//in any cpp file
MAP_REG_DEFINE_STR(MapName2, a, 1);
MAP_REG_DEFINE_STR(MapName2, abc, 2);



静态运行使用例子：
STATIC_RUN() //可以任意CPP文件定义
{
	... --your code
}

*/
#pragma once


#include <set>
#include <map>
#include <vector>
#include <string>

//------------------------------------set 静态注册----------------------------------------------
//h文件定义唯一单件注册类 SET
#define REG_SET_NAME_DECLARE(SetClassName, ValueType)\
class SetClassName : public std::set<ValueType>\
{\
public:\
	static SetClassName &Obj()\
	{\
		static SetClassName d;\
		return d;\
	}\
	struct RunReg \
		{\
		RunReg(ValueType para)\
			{ \
			SetClassName::Obj().insert(para).second; \
			};\
		}; \
private:\
	SetClassName(){};\
};


//以下宏定义可以在不同文件里面调用，实现不同cpp文件写注册功能。
//@SetClassName(单件注册类), 
//@value(注册的值)
#define SET_REG_DEFINE(SetClassName, value) \
	namespace{SetClassName::RunReg SetClassName_##value(value);}

#define SET_REG_NAME_DEFINE(SetClassName, obj_name, parameter) \
	namespace{SetClassName::RunReg SetClassName_##obj_name(parameter);}

//------------------------------------map 静态注册----------------------------------------------
//h文件定义唯一单件注册类 MAP
#define REG_MAP_NAME_DECLARE(MapClassName, KeyType, MapType)\
class MapClassName : public std::map<KeyType, MapType>\
{\
public:\
	static MapClassName &Obj()\
	{\
	static MapClassName d; \
	return d; \
	}\
	struct RunReg \
	{\
		RunReg(const std::pair<KeyType, MapType> &para)\
		{ \
			MapClassName::Obj().insert(para).second; \
		};\
	}; \
private:\
	MapClassName(){};\
};

//static_cast有更强的编译期错误检查
#define MAP_REG_NAME_DEFINE(MapClassName, obj_name, key, map_value) \
	namespace{MapClassName::RunReg MapClassName##obj_name(make_pair(\
static_cast<MapClassName::key_type>(key),\
static_cast<MapClassName::value_type::second_type>(map_value)\
));}

//注册值，MapClassName(单件注册类), key(键值字符串，不用"") map_value(值)
#define MAP_REG_DEFINE_STR(MapClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(MapClassName, key, #key, map_value)

//注册值，MapClassName(单件注册类), key(键值, INT) map_value(值)
#define MAP_REG_DEFINE(MapClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(MapClassName, key, key, map_value)


#define STATIC_RUN_CAT(name) \
namespace {struct StaticRunReg##name\
{\
	StaticRunReg##name();\
};\
static StaticRunReg##name _StaticRunReg##name;}\
StaticRunReg##name::StaticRunReg##name()

#define STATIC_RUN_LINE(line) STATIC_RUN_CAT(line)  //多一层，避免__LINE__直接变化成字符串"__LINE__"
#define STATIC_RUN() STATIC_RUN_LINE(__LINE__)