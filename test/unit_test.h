/*
version: 1.02
�򵥵ĵ�Ԫ���Թ��ܡ�
	 �﷨���
	 �����ʾ�ڱ�׼���������ѡ������Լ�����־ʵ�֡�
	 �����쳣����
	 ������Ψһ������

ʹ�÷���������unit_test.h *.cpp�ļ�����Ĺ��̣�����ʹ�á�
excamples:

UNITTEST(t1)
{
	UNIT_ASSERT(1 == 11);

	UNIT_INFO("run t1");
}

int main()
{
UnitTestMgr::Obj().Start();
}

*/

#pragma once

#include <vector>
#include <stdarg.h>
#include <map>

class IUnitTest
{
public:
	IUnitTest(const char *unit_name);
	virtual void Run() = 0;

	const char *m_unit_name = "";
};

//@para va_list vp, vp����Ҫ�ص������ͷ�
using UnitTestPrintf = void (*)(bool is_error, const char * file, int line, const char *fun, const char * pattern, va_list vp);
class UnitTestMgr
{
public:
	static UnitTestMgr &Obj()
	{
		static UnitTestMgr d;
		return d;
	}
	void Start(UnitTestPrintf printf= nullptr);
	void Reg(IUnitTest *p);
	void Printf(bool is_error, const char * file, int line, const char *pFun, const char * pattern, ...);

private:
	UnitTestMgr()
		:m_print(nullptr)
	{}

private:
	std::map < std::string, IUnitTest* > m_name2unit;
	UnitTestPrintf m_print;
};

#define UNIT_ERROR(x, ...)  UnitTestMgr::Obj().Printf( true, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define UNIT_INFO(x, ...)  UnitTestMgr::Obj().Printf( false, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);

#define UNIT_ASSERT(expression) do{  \
				if(!(expression))                                                              \
				{\
					UNIT_ERROR(#expression);		\
					std::exception  e; throw e;	\
				}\
			}while(0)                                                   

//ֻ����־�����쳣
#define UNIT_CHECK(expression) do{  \
				if(!(expression))                                                              \
				{\
					UNIT_ERROR(#expression);		\
				}\
			}while(0)     


#define UNITTEST(Name)                                                   \
   class Test##Name : public IUnitTest                                            \
   {                                                                                     \
   public:                                                                               \
      Test##Name():IUnitTest(#Name){} \
   private:                                                                              \
      virtual void Run();                                                      \
   };                                         \
    namespace { Test##Name  test##Name##Obj;}                                   \
   void Test##Name::Run()
