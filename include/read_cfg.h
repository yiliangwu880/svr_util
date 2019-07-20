/*
brief:��ȡ���ı���ʽ���ã�����Ҫ�������⣬���Կ��ټ��빤��ʹ�á�
�����ļ���ʽ1��nameҪ��C�����������value�����ַ���
name=value
name = 3
�����ļ���ʽ2�������ʽ���£�(ֵ��֧����,=�ַ�)
name:v0,v1,v2 ...
*/
#pragma once
#include "typedef.h"
#include "cnt_typedef.h"
#include <utility>

class Config
{
public:
    bool init(const char *file_name);

    const char *GetStr(const char *name);
    int GetInt(const char *name);
    uint64 GetUint64(const char *name);
    void GetStrArray(const char *name, VecStr &vec_str);
    void GetIntArray(const char *name, VecInt &vec_int);
    //��ȡ�ظ��������������棬ͨ������ˡ�
    const VecStr &GetRepeatName();
private:
    Str2Str m_str_2_str;
    Str2Str m_str_2_array;
    VecStr m_repeat_str;
};