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
MAP_REG_DEFINE(MapName2, a, 1);
MAP_REG_DEFINE(MapName2, abc, 2);

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


#define SET_REG_LINENAME_CAT(name, line) name##line
#define SET_REG_LINENAME(name, line) SET_REG_LINENAME_CAT(name, line) 


//���º궨������ڲ�ͬ�ļ�������ã�ʵ�ֲ�ͬcpp�ļ�дע�Ṧ�ܡ�
//@SetClassName(����ע����), 
//@value(ע���ֵ)
#define SET_REG_DEFINE(SetClassName, value) \
	namespace{SetClassName::RunReg SET_REG_LINENAME(s##value, __LINE__)(value);}

#define SET_REG_NAME_DEFINE(SetClassName, obj_name, parameter) \
	namespace{SetClassName::RunReg s_##obj_name(parameter);}

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

//��һ�㣬����__LINE__ֱ�ӱ仯���ַ���"__LINE__"
#define MAG_REG_LINENAME_CAT(name, line) name##line
#define MAG_REG_LINENAME(name, line) MAG_REG_LINENAME_CAT(name, line) 

#define MAP_REG_NAME_DEFINE(SetClassName, obj_name, key, map_value) \
	namespace{SetClassName::RunReg MAG_REG_LINENAME(obj_name, __LINE__)(make_pair(SetClassName::key_type(key), SetClassName::value_type::second_type(map_value)));}

//ע��ֵ��SetClassName(����ע����), key(��ֵ�ַ���������"") map_value(ֵ)
#define MAP_REG_DEFINE(SetClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(SetClassName, key, #key, map_value)





