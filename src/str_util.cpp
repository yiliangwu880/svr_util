#include "str_util.h"
#include <algorithm>

using namespace std;
namespace su
{


	std::string StrUtil::BinaryToHex(const std::string &binaryStr)
	{
		string ret;
		static const char *hex = "0123456789ABCDEF";
		for (auto c : binaryStr)
		{
			ret.push_back(hex[(c >> 4) & 0xf]); //取二进制高四位
			ret.push_back(hex[c & 0xf]);        //取二进制低四位
		}
		return ret;
	}

void StrFormat::format(std::string& resultOUT, const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);

	resultOUT.resize(1024, 0);
	int len;
	for (;;)
	{
		len = vsnprintf((char *)resultOUT.c_str(), resultOUT.size(), fmt, vl);
		if (len == -1)
		{
			resultOUT.clear();
			break;
		}
		if (len >= (int)resultOUT.size())
		{
			resultOUT.resize(resultOUT.size() * 2, 0);
			va_start(vl, fmt);
			continue;
		}
		break;
	}
	resultOUT.resize(len);

	va_end(vl);
}

std::string StrFormat::format(const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	std::string resultOUT;

	resultOUT.resize(1024, 0);
	int len;
	for (;;)
	{
		len = vsnprintf((char *)resultOUT.c_str(), resultOUT.size(), fmt, vl);
		if (len == -1)
		{
			resultOUT.clear();
			break;
		}
		if (len >= (int)resultOUT.size())
		{
			resultOUT.resize(resultOUT.size() * 2, 0);
			va_start(vl, fmt);
			continue;
		}
		break;
	}
	resultOUT.resize(len);


	va_end(vl);
	return resultOUT;
}


void StrUtil::split(const std::string& str, std::string::value_type separator,VecStr& resultOUT,size_t max_out_size)
{
    resultOUT.clear();
    if(max_out_size == 0) 
        return;
    resultOUT.push_back(std::string());
    for(std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if(*it == separator)
        {
            if(!resultOUT.back().empty())
            {
                if(resultOUT.size() == max_out_size) 
                    return;
                resultOUT.push_back(std::string());
            }
        }
        else
        {
            resultOUT.back().push_back(*it);
        }
    }
    if(resultOUT.back().empty())
        resultOUT.pop_back();
}

void StrUtil::split(const std::string& src_str, const std::string& separator, VecStr& out, size_t max_out_size)
{
    out.clear();
    if(max_out_size == 0) 
        return;
    if(separator.empty())
    {
        out.push_back(src_str);
        return;
    }
    std::string::size_type begin = 0, index;
    for(;;)
    {
        index = src_str.find(separator, begin);
        if(index == std::string::npos)
            break;
        size_t len = index - begin;
        if(len != 0)
        {
            out.push_back(src_str.substr(begin, len));
            if(out.size() == max_out_size)
                return;
        }
        begin = index + separator.size();
    }
    if(begin < src_str.size() && out.size() < max_out_size)
        out.push_back(src_str.substr(begin));
}

void StrUtil::split(const std::string& str, const VecStr& separatorList, VecStr& out, size_t max_out_size)
{
    out.clear();
    if(max_out_size == 0) 
        return;
    if(separatorList.empty())
    {
        out.push_back(str);
        return;
    }

    std::string::size_type begin = 0;
    for(std::string::size_type i = 0; i < str.size(); i++)
    {
        for(VecStr::const_iterator it = separatorList.begin(); it != separatorList.end(); ++it)
        {
            if(it->compare(0, it->size(), str, i, it->size()) == 0)
            {
                if(i != begin)
                {
                    out.push_back(str.substr(begin, i - begin));
                    if(out.size() == max_out_size)
                        return;
                }
                begin = i + it->size();
                i = begin - 1;
                break;
            }
        }
    }
    if(begin < str.size() && out.size() < max_out_size)
        out.push_back(str.substr(begin));
}

void StrUtil::split(const char* str, std::string::value_type separator, VecStr& out, size_t max_out_size)
{
    std::string temp = str;
    StrUtil::split(temp, separator, out, max_out_size);
}

void StrUtil::replace( std::string& str, const std::string& old_value, const std::string& new_value )
{
    if (old_value.empty())
    {
        return;
    }
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   
    {     
        if(   (pos=str.find(old_value,pos))!=string::npos   )     
            str.replace(pos,old_value.length(),new_value);     
        else   break;     
    }        
}

void StrUtil::replace( std::string& str, char old_value, char new_value )
{
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=1)   
    {     
        if(   (pos=str.find(old_value,pos))!=string::npos   )     
            str.replace(pos, 1, 1 ,new_value);     
        else   break;     
    }   
}


void StrUtil::tolower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

}