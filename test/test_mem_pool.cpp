/*
brief: ¾²Ì¬×¢²áÊ¹ÓÃÀý×Ó
*/

#include "static_trick/static_reg.h"
#include "unit_test.h"
#include "misc.h"
#include <iostream>
#include <map>
#include <string>
#include "stl_boost.h"
#include "mem_pool.h"

using namespace std;
using namespace su;

namespace
{
	bool g_create = false;
	bool g_destory = false;
	struct T1 : public MemPoolBaseObj<T1, 2>
	{
		T1() 
		{
			g_create = true;
		}
		~T1()
		{
			g_destory = true;
		}
	}; 



}

UNITTEST(mem_pool)
{
	
	T1 *p = new T1;
	UNIT_ASSERT(g_create);

	UNIT_ASSERT(T1::GetAllocateNum() == 1);

	T1 *p2 = new T1;
	UNIT_ASSERT(T1::GetAllocateNum() == 2);
	T1 *p3 = new T1;
	UNIT_ASSERT(T1::GetAllocateNum() == 3);

	UNIT_ASSERT(!g_destory);
	delete (p3);
	UNIT_ASSERT(g_destory);
	UNIT_ASSERT(T1::GetAllocateNum() == 2);
	delete (p);
	delete (p2);
	UNIT_ASSERT(T1::GetAllocateNum() == 0);

}