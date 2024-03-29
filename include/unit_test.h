/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/svr_util.git
version: 1.03
简单的单元测试功能。
	 语法简洁
	 结果显示在标准输出，可以选择接入自己的日志实现。
	 错误异常处理
	 测试名唯一，排序

使用方法：复制unit_test.h *.cpp文件到你的工程，编译使用。
excamples:

UNITTEST(t1)
{
	UNIT_ASSERT(1 == 11);

	UNIT_INFO("run t1");
}

int main()
{
UnitTestMgr::Ins().Start();
}

*/

#pragma once

#include <vector>
#include <stdarg.h>
#include <map>

class IUnitTest
{
public:
	IUnitTest(const char *unit_name, bool isSingle = false);
	virtual void Run() = 0;

	const char *m_unit_name = "";
};

//@para va_list vp, vp不需要回调里面释放
using UnitTestPrintf = void (*)(bool is_error, const char * file, int line, const char *fun, const char * pattern, va_list vp);
class UnitTestMgr
{
	std::map < std::string, IUnitTest* > m_name2unit;
	UnitTestPrintf m_print = nullptr;
	bool m_isEnable = true;
	IUnitTest* m_singleUnit = nullptr;

public:
	static UnitTestMgr &Ins()
	{
		static UnitTestMgr d;
		return d;
	}
	void Start(UnitTestPrintf printf = nullptr);
	void Run(const std::string &testName, UnitTestPrintf printf = nullptr);//只运行测试一个注册模块
	void Reg(IUnitTest *p);
	void RegSingle(IUnitTest *p);//独立测试，会忽略Reg函数的注册
	void Printf(bool is_error, const char * file, int line, const char *pFun, const char * pattern, ...);
	void Enable(bool isEnalbe) { m_isEnable = isEnalbe; } //fasle == isEnalbe表示不打日志

};

#define UNIT_ERROR(x, ...)  UnitTestMgr::Ins().Printf( true, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__)
#define UNIT_INFO(x, ...)  UnitTestMgr::Ins().Printf( false, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__)

#define UNIT_ASSERT(expression) do{  \
				if(!(expression))                                                              \
				{\
					UNIT_ERROR(#expression);		\
					std::exception  e; throw e;	\
				}\
			}while(0)                                                   

//只出日志，不异常
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
    namespace { Test##Name  test##Name##Ins;}                                   \
   void Test##Name::Run()


#define UNITTEST_SINGLE(Name)                                                   \
   class Test##Name : public IUnitTest                                            \
   {                                                                                     \
   public:                                                                               \
      Test##Name():IUnitTest(#Name, true){} \
   private:                                                                              \
      virtual void Run();                                                      \
   };                                         \
    namespace { Test##Name  test##Name##Ins;}                                   \
   void Test##Name::Run()
