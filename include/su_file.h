/*
author: YiliangWu

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "singleton.h"
#include "typedef.h"
#include "cnt_typedef.h"


namespace su
{
	class File
	{
	public:
		static void Str2File(const std::string &file_name, const std::string &content);
		static void File2Str(const std::string &file_name, std::string &content);
	};
}

//end file
