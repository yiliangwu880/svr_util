/*
author:yiliangwu
brief: 静态注册。
实际项目使用，参考下这里代码，自写一些自定义代码，会更好阅读。 比如写个单例，定义个注册宏就行了。
特点:
	注册可以写在cpp任意地方,免去到处翻代码文件添加声明或者定义
	需要注意：main函数后才保证初始化成功。

缺点：实现原理不好理解

use example:

STATIC_RUN(int i=3;) //可以任意CPP文件,函数外定义。 语句会再main函数前执行


*/
#pragma once


#include <set>
#include <map>
#include <vector>
#include <string>

#define STATIC_RUN_CAT(name, exe)\
namespace {\
	struct StaticRunReg##name\
	{\
		StaticRunReg##name()\
		{\
		 exe;\
		}\
	};\
	static StaticRunReg##name _StaticRunReg##name;\
}

 //多一层，避免__LINE__直接变化成字符串"__LINE__"
#define STATIC_RUN_LINE(line, exe) STATIC_RUN_CAT(line, exe)
#define STATIC_RUN(exe) STATIC_RUN_LINE(__LINE__, exe)
