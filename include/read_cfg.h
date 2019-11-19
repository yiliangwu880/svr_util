/*
brief:读取简单文本格式配置，不需要第三方库，可以快速加入工程使用。
注释用 #号
配置文件格式1：name要求C语言命名风格，value任意字符串
name=value  #这里是注释
name = 3
配置文件格式2：数组格式如下：(值不支持有,=字符)
name:v0,v1,v2 ...

注意： 发现win格式的文档会读取失败，待解决
*/
#pragma once
#include "typedef.h"
#include "cnt_typedef.h"
#include <utility>
namespace su
{
class Config
{
public:
    bool init(const char *file_name);

    const char *GetStr(const char *name);
    int GetInt(const char *name);
	uint64 GetUint64(const char *name);
	double GetDouble(const char *name);
    void GetStrArray(const char *name, VecStr &vec_str);
    void GetIntArray(const char *name, VecInt &vec_int);
    //获取重复变量名，给警告，通常填错了。
    const VecStr &GetRepeatName();
private:
    Str2Str m_str_2_str;
    Str2Str m_str_2_array;
    VecStr m_repeat_str;
};
}