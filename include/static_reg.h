/*
author:yiliangwu
brief: ��̬ע��ʹ������, ��̬����
ʵ����Ŀʹ�ã��ο���������룬��дһЩ�Զ�����룬������Ķ��� ����д�������������ע�������ˡ�
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



��̬����ʹ�����ӣ�
STATIC_RUN(int i=3;) //��������CPP�ļ����� 


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
	static SetClassName &Ins()\
	{\
		static SetClassName d;\
		return d;\
	}\
	struct RunReg \
		{\
		RunReg(ValueType para)\
			{ \
			SetClassName::Ins().insert(para).second; \
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
#define REG_MAP_NAME_DECLARE(MapClassName, KeyType, MapType)\
class MapClassName : public std::map<KeyType, MapType>\
{\
public:\
	static MapClassName &Ins()\
	{\
	static MapClassName d; \
	return d; \
	}\
	struct RunReg \
	{\
		RunReg(const std::pair<KeyType, MapType> &para)\
		{ \
			MapClassName::Ins().insert(para).second; \
		};\
	}; \
private:\
	MapClassName(){};\
};

//static_cast�и�ǿ�ı����ڴ�����
#define MAP_REG_NAME_DEFINE(MapClassName, obj_name, key, map_value) \
	namespace{MapClassName::RunReg MapClassName##obj_name(make_pair(\
static_cast<MapClassName::key_type>(key),\
static_cast<MapClassName::value_type::second_type>(map_value)\
));}

//ע��ֵ��MapClassName(����ע����), key(��ֵ�ַ���������"") map_value(ֵ)
#define MAP_REG_DEFINE_STR(MapClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(MapClassName, key, #key, map_value)

//ע��ֵ��MapClassName(����ע����), key(��ֵ, INT) map_value(ֵ)
#define MAP_REG_DEFINE(MapClassName, key, map_value) \
	MAP_REG_NAME_DEFINE(MapClassName, key, key, map_value)


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

 //��һ�㣬����__LINE__ֱ�ӱ仯���ַ���"__LINE__"
#define STATIC_RUN_LINE(line, exe) STATIC_RUN_CAT(line, exe)
#define STATIC_RUN(exe) STATIC_RUN_LINE(__LINE__, exe)

//------------------------------��ʵ��,��������Ķ�
/*
ʹ������
using StaticReg1 = StaticReg<int, int, 1>;
namespace
{ 
	StaticReg1::RunReg kd141(22, 333);
}
void test()
{
	LOG( (StaticReg1::Ins()[22]));
}
*/

template<class KeyType, class MapType, int Id>
class StaticReg : public std::map<KeyType, MapType>
{
public:
	static StaticReg &Ins()
	{
		static StaticReg d;
		return d;
	}
	struct RunReg
	{
		RunReg(KeyType key, MapType value)
		{
			StaticReg::Ins().insert(std::make_pair(key, value));
		};
	};
private:
	StaticReg() {};
};