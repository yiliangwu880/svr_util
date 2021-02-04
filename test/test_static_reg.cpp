/*
brief: ¾²Ì¬×¢²áÊ¹ÓÃÀý×Ó
*/

#include "game_util/static_reg.h"
#include "unit_test.h"
#include "misc.h"
#include <iostream>
#include <map>
#include <string>
#include "stl_boost.h"

using namespace std;
using namespace su;

namespace
{

	REG_SET_NAME_DECLARE(MyName, int)
	SET_REG_DEFINE(MyName, 0);
	SET_REG_DEFINE(MyName, 1);
	SET_REG_DEFINE(MyName, 2);

	REG_SET_NAME_DECLARE(MyName2, int)
	SET_REG_DEFINE(MyName2, 20);
	SET_REG_DEFINE(MyName2, 21);
	SET_REG_DEFINE(MyName2, 22);

	REG_MAP_NAME_DECLARE(MapName2, string, int)
	MAP_REG_NAME_DEFINE(MapName2, g_a, "a", 1);
	MAP_REG_NAME_DEFINE(MapName2, g_b, "abc", 2);
	MAP_REG_NAME_DEFINE(MapName2, g_b2, "accd", 2);
	MAP_REG_DEFINE_STR(MapName2, bda, 2);
	MAP_REG_DEFINE_STR(MapName2, ca, 2);



}
UNITTEST(test_static_reg)
{

	UNIT_ASSERT(MyName::Obj().size() == 3);
	
	UNIT_ASSERT(MyName::Obj().insert(0).second == false);
	UNIT_ASSERT(MyName::Obj().insert(1).second == false);
	UNIT_ASSERT(MyName::Obj().insert(2).second == false);
	UNIT_ASSERT(MyName::Obj().insert(3).second == true);


	UNIT_ASSERT(MyName2::Obj().size() == 3);
	UNIT_ASSERT(MyName2::Obj().insert(20).second == false);
	UNIT_ASSERT(MyName2::Obj().insert(21).second == false);
	UNIT_ASSERT(MyName2::Obj().insert(22).second == false);
	UNIT_ASSERT(MyName2::Obj().insert(23).second == true);

	UNIT_ASSERT(MapName2::Obj().size() == 5);
	UNIT_ASSERT(MapInsert(MapName2::Obj(), "a", 1) == false);
	UNIT_ASSERT(MapInsert(MapName2::Obj(), "adfdfd2", 1) == true);


}