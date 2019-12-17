/*
author:yiliangwu
brief: ��̬ע��ʹ������
�ص�:
	ע�����д��cpp����ط�,��ȥ�����������ļ�����������߶���
	main������ű�֤��ʼ���ɹ���

ȱ�㣺ʵ��ԭ�������

use example:
//in h file
REG_MAP_NAME_DECLARE(MapName2, string, int)

//in any cpp file
MAP_REG_DEFINE_STR(MapName2, a, 1);
MAP_REG_DEFINE_STR(MapName2, abc, 2);

*/
#pragma once


#include <set>
#include <map>
#include <vector>
#include <string>

//------------------------------------set ��̬ע��----------------------------------------------
//h�ļ�����Ψһ����ע���� SET
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


//���º궨������ڲ�ͬ�ļ�������ã�ʵ�ֲ�ͬcpp�ļ�дע�Ṧ�ܡ�
//@SetClassName(����ע����), 
//@value(ע���ֵ)
#define SET_REG_DEFINE(SetClassName, value) \
	namespace{SetClassName::RunReg SetClassName_##value(value);}

#define SET_REG_NAME_DEFINE(SetClassName, obj_name, parameter) \
	namespace{SetClassName::RunReg SetClassName_##obj_name(parameter);}

//------------------------------------map ��̬ע��----------------------------------------------
//h�ļ�����Ψһ����ע���� MAP
#define REG_MAP_NAME_DECLARE(SetClassName, KeyType, MapType)\
class SetClassName : public std::map<KeyType, MapType>\
{\
public:\
	static SetClassName &Obj()\
	{\
	static SetClassName d; \
	return d; \
	}\
	struct RunReg \
	{\
		RunReg(const std::pair<KeyType, MapType> &para)\
		{ \
			SetClassName::Obj().insert(para).second; \
		};\
	}; \
private:\
	SetClassName(){};\
};

//static_cast�и�ǿ�ı����ڴ�����
#define MAP_REG_NAME_DEFINE(SetClassName, obj_name, key, map_value) \
	namespace{SetClassName::RunReg SetClassName##obj_name(make_pair(\
static_cast<SetClassName::key_type>(key),\
static_cast<SetClassName::value_type::second_type>(map_value)\
));}

//ע��ֵ��SetClassName(����ע����), key(��ֵ�ַ���������"") map_value(ֵ)
#define MAP_REG_DEFINE_STR(SetClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(SetClassName, key, #key, map_value)

//ע��ֵ��SetClassName(����ע����), key(��ֵ, INT) map_value(ֵ)
#define MAP_REG_DEFINE(SetClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(SetClassName, key, key, map_value)



