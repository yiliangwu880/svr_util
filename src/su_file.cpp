#include "su_file.h"
#include "log_def.h"
#include <fstream>

using namespace su;
using namespace std;

void su::File::Str2File(const std::string &file_name, const std::string &content)
{
	ofstream ofs(file_name, ios::ate | ios::out);
	if (!ofs.is_open())
	{
		L_ERROR("open file for write fail: %s", file_name.c_str());
		return;
	}
	ofs << content;
	ofs.close();
}

void su::File::File2Str(const std::string &file_name, std::string &content)
{
	content.clear();
	std::ifstream t(file_name);
	if (!t.is_open())
	{
		L_ERROR("open file for read fail: %s", file_name.c_str());
		return;
	}
	content.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	t.close();                    // close file handle
}
