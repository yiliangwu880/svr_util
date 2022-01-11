#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <limits.h>
#include <errno.h>
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include "cnt_typedef.h"

namespace su
{
	struct StrNum
	{
		template<typename T>
		static std::string NumToStr(T num);
		template<typename T>
		static T StrToNum(const std::string &str, bool *is_success = nullptr);
		//用下面代替StrToNum更简洁
		static int32 ToInt32(const std::string& str, bool* is_success = nullptr);
		static int32 ToInt64(const std::string& str, bool* is_success = nullptr);
		static int32 ToUint32(const std::string& str, bool* is_success = nullptr);
		static int32 ToUint64(const std::string& str, bool* is_success = nullptr);
	};

	struct StrFormat
	{
		static const int DEFAULT_BUFFER_SIZE = 1024;
		static void format(std::string& resultOUT, const char* fmt, ...);
		static std::string format(const char* fmt, ...);
		static std::string& vformat(std::string& resultOUT, const char *fmt, va_list argptr, int default_size = DEFAULT_BUFFER_SIZE);
		static std::string vformat(const char *fmt, va_list argptr, int default_size = DEFAULT_BUFFER_SIZE);
	};
	
	struct StrUtil
	{
		static const std::string WHITE_SPACE;

		//任意字符串转可视的十六进制
		static std::string BinaryToHex(const std::string &binaryStr);

		static std::string Trimbegin(std::string& str, const std::string& whiteSpace = WHITE_SPACE);
		static std::string Trimend(std::string& str, const std::string& whiteSpace = WHITE_SPACE);
		static std::string Trim(std::string& str, const std::string& whiteSpace = WHITE_SPACE);

		static bool IsSubStr(const std::string& str, const std::string& tosearch);
		static bool IsSubStr(const std::string& str, const char* tosearch);

		static void split(const std::string& str, std::string::value_type separator, VecStr& out, size_t max_out_size = 2000 * 200);
		static void split(const std::string& str, const std::string& separator, VecStr& out, size_t max_out_size = 2000 * 200);
		static void split(const std::string& str, const VecStr& separatorList, VecStr& out, size_t max_out_size = 2000 * 200);
		static void split(const char* str, std::string::value_type separator, VecStr& out, size_t max_out_size = 2000 * 200);

		//@from  被替换子字符串
		//@to    替换成字符
		//@#str 输入字符串，输出替换后字符串。
		static void replace(std::string& str, const std::string& from, const std::string& to);
		static void replace(std::string& str, char from, char to);

		static void erase(std::string& str, const std::string& to_remove);
		static void erase(std::string& str, char c);

		static bool is_lower(const std::string& str);
		static bool is_lower(char c);
		static bool is_upper(const std::string& str);
		static bool is_upper(char c);
		static bool is_alpha(const std::string& str);
		static bool is_alpha(char c);
		static bool is_numeric(const std::string& str);
		static bool is_numeric(char c);
		static bool is_alnum(const std::string& str);
		static bool is_alnum(char c);

		static void tolower(std::string& str);
		static char* tolower(char* pstr);
		static char tolower(char c);
		static std::string& toupper(std::string& str);
		static char toupper(char c);
	};

	inline std::string StrFormat::vformat(const char *fmt, va_list argptr, int default_size)
	{
		std::string resultOUT;
		return vformat(resultOUT, fmt, argptr, default_size);
	}

	inline std::string StrUtil::Trimbegin(std::string& str, const std::string& whiteSpace)
	{
		return str.erase(0, str.find_first_not_of(whiteSpace));
	}
	inline std::string StrUtil::Trimend(std::string& str, const std::string& whiteSpace)
	{
		return str.erase(str.find_last_not_of(whiteSpace) + 1);
	}
	inline std::string StrUtil::Trim(std::string& str, const std::string& whiteSpace)
	{
		str.erase(str.find_last_not_of(whiteSpace) + 1);
		return str.erase(0, str.find_first_not_of(whiteSpace));
	}
	inline bool StrUtil::IsSubStr(const std::string& str, const std::string& tosearch)
	{
		return str.find(tosearch) != std::string::npos;
	}
	inline bool StrUtil::IsSubStr(const std::string& str, const char* tosearch)
	{
		if (tosearch == NULL)
			return false;
		return str.find(tosearch) != std::string::npos;
	}



	inline void StrUtil::erase(std::string& str, const std::string& to_remove)
	{
		replace(str, to_remove, "");
	}

	inline void StrUtil::erase(std::string& str, char c)
	{
		std::string s(1, c);
		replace(str, s, "");
	}

	inline bool StrUtil::is_lower(char c)
	{
		return c >= 'a' && c <= 'z';
	}
	inline bool StrUtil::is_upper(char c)
	{
		return c >= 'A' && c <= 'Z';
	}
	inline bool StrUtil::is_alpha(char c)
	{
		return is_lower(c) || is_upper(c);
	}
	inline bool StrUtil::is_numeric(char c)
	{
		return c >= '0' && c <= '9';
	}
	inline bool StrUtil::is_alnum(char c)
	{
		return is_alpha(c) || is_numeric(c);
	}

	inline char StrUtil::tolower(char c)
	{
		return is_upper(c) ? c - 'A' + 'a' : c;
	}
	inline char StrUtil::toupper(char c)
	{
		return is_lower(c) ? c - 'a' + 'A' : c;
	}



	template<typename T>
	inline std::string su::StrNum::NumToStr(T num)
	{
		return std::to_string(num);
	}	
	template<>
	inline std::string su::StrNum::NumToStr<bool>(bool num)
	{
		return num?"true":"false";
	}

	namespace inner
	{

		template<typename T>
		inline T StrToNumEx(const std::string &str, bool *is_success)
		{
			try {
				if (nullptr != is_success)
				{
					*is_success = true;
				}
				return std::stoi(str);
			}
			catch (const std::invalid_argument&) {
				if (nullptr != is_success)
				{
					*is_success = false;
				}
				return 0;
			}
		}

		template<>
		inline double StrToNumEx<double>(const std::string &str, bool *is_success)
		{
			try {
				if (nullptr != is_success)
				{
					*is_success = true;
				}
				return std::stod(str);
			}
			catch (const std::invalid_argument&) {
				if (nullptr != is_success)
				{
					*is_success = false;
				}
				return 0;
			}
		}

		template<>
		inline float StrToNumEx<float>(const std::string &str, bool *is_success)
		{
			try {
				if (nullptr != is_success)
				{
					*is_success = true;
				}
				return std::stof(str);
			}
			catch (const std::invalid_argument&) {
				if (nullptr != is_success)
				{
					*is_success = false;
				}
				return 0;
			}
		}
		template<>
		inline long StrToNumEx<long>(const std::string &str, bool *is_success)
		{
			try {
				if (nullptr != is_success)
				{
					*is_success = true;
				}
				return std::stol(str);
			}
			catch (const std::invalid_argument&) {
				if (nullptr != is_success)
				{
					*is_success = false;
				}
				return 0;
			}
		}
		template<>
		inline long long StrToNumEx<long long>(const std::string &str, bool *is_success)
		{
			try {
				if (nullptr != is_success)
				{
					*is_success = true;
				}
				return std::stoll(str);
			}
			catch (const std::invalid_argument&) {
				if (nullptr != is_success)
				{
					*is_success = false;
				}
				return 0;
			}
		}
		template<>
		inline unsigned long  StrToNumEx<unsigned long >(const std::string &str, bool *is_success)
		{
			try {
				if (nullptr != is_success)
				{
					*is_success = true;
				}
				return std::stoul(str);
			}
			catch (const std::invalid_argument&) {
				if (nullptr != is_success)
				{
					*is_success = false;
				}
				return 0;
			}
		}
		template<>
		inline unsigned long long StrToNumEx<unsigned long long>(const std::string &str, bool *is_success)
		{
			try {
				if (nullptr != is_success)
				{
					*is_success = true;
				}
				return std::stoull(str);
			}
			catch (const std::invalid_argument&) {
				if (nullptr != is_success)
				{
					*is_success = false;
				}
				return 0;
			}
		}




	}
	template<typename T>
	T su::StrNum::StrToNum(const std::string &str, bool *is_success)
	{
		return su::inner::StrToNumEx<T>(str, is_success);
	}
}