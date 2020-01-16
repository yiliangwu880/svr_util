
#include "log_def.h"
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;
namespace su
{
void LogMgr::SetLogPrinter(ILogPrinter &iprinter)
{
	m_iprinter = &iprinter;
}

void LogMgr::Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, ...)
{
	va_list vp;
	va_start(vp, pattern);
	GetILogPrinter().Printf(lv, file, line, fun, pattern, vp);
	va_end(vp);
}

void LogMgr::Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp)
{
	GetILogPrinter().Printf(lv, file, line, fun, pattern, vp);
}


void LogMgr::PrintfCond(LogLv lv, const char * file, int line, const char *fun, const char * cond, const char * pattern/*=""*/, ...)
{
	std::string fmt = cond;
	fmt.append(pattern);
	va_list vp;
	va_start(vp, pattern);
	GetILogPrinter().Printf(lv, file, line, fun, fmt.c_str(), vp);
	va_end(vp);
}

void LogMgr::flush()
{
	GetILogPrinter().flush();
}

LogMgr::LogMgr()
	:m_iprinter(nullptr)
{
}

su::ILogPrinter &LogMgr::GetILogPrinter()
{
	if (nullptr == m_iprinter)
	{// 使用默认才创建s_log对象
		static DefaultLog s_log;
		m_iprinter = &s_log;
	}
	return *m_iprinter;
}

void DefaultLog::flush()
{

}

void DefaultLog::setStdOut(bool is_std_out)
{
	m_is_std_out = is_std_out;
}


DefaultLog::DefaultLog(const char *fname)
	:m_log_lv(LL_TRACE)
	, m_fd(-1)
	, m_is_std_out(true)
	, m_file_name(fname)
{
	OpenFile();
}



void DefaultLog::setShowLv(LogLv lv)
{
	m_log_lv = lv;
}

DefaultLog::~DefaultLog()
{
	if (-1 != m_fd)
	{
		close(m_fd);
	}
}

void DefaultLog::Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp)
{
	char line_str[10];
	snprintf(line_str, sizeof(line_str), "%d", line);

	//add time infomation
	char time_str[1000];
	{
		time_t long_time;
		time(&long_time);
		tm   *now;
		now = localtime(&long_time);
		snprintf(time_str, sizeof(time_str), "[%04d-%02d-%02d %02d:%02d:%02d] ", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	}

	string s;
	s.append(time_str);
	s.append(" ");
	s.append(GetLogLevelStr(lv));
	s.append(pattern);
	//if (lv <= m_log_lv)
	{
		s.append("  --");
		s.append(file);
		s.append(":");
		s.append(line_str);
		s.append(" ");
		s.append(fun);
	}
	s.append("\n");



	bool is_exit = (access(m_file_name.c_str(), F_OK | W_OK) == 0);
	if (!is_exit)
	{
		close(m_fd);
		m_fd = -1;
		OpenFile();
	}


	if (m_is_std_out)
	{
		char out_str[1000];
		int r = vsnprintf(out_str, sizeof(out_str), s.c_str(), vp);
		//用一次vfprintf，再用vprintf有时候有BUG， vp被 vfprintf修改了，原因未明
		::write(m_fd, out_str, r);
		::puts(out_str);		
		if (r > (int)sizeof(out_str))
		{
			dprintf(m_fd, "....[str too long,len=%d]\n", r);
			::printf("....[str too long,len=%d]\n", r);
		}
	}
	else
	{
		vdprintf(m_fd, s.c_str(), vp);
	}

}

const char * DefaultLog::GetLogLevelStr(LogLv lv) const
{
	switch (lv)
	{
	default:
		return "[unknow]";
		break;
	case LL_FATAL:
		return "[fatal] ";
		break;
	case LL_ERROR:
		return "[error] ";
		break;
	case LL_WARN:
		return "[warn]  ";
		break;
	case LL_DEBUG:
		return "[debug] ";
		break;
	case LL_INFO:
		return "[info]  ";
		break;
	case LL_TRACE:
		return "[trace]   ";
		break;
	}
	return "[unknow]";
}



void DefaultLog::OpenFile()
{
	// 打开文件，不存在则创建
	m_fd = open(m_file_name.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (m_fd < 0)
	{
		::printf("fail open file[%s]\n", m_file_name.c_str());
	}

}

 }