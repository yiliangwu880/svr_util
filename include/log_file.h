/*
	写库时需要打印日志，同时给使用库的用户提供改变日志实现的选择。
	日志宏在 log_def.h.  用来做库时，一般不给用户用log_def.h。 用户自己定义新的宏来定义新的实现

用户改变日志实现例子：
class MyLog : public ILogPrinter
{
public:
	virtual void Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp) override
	{
			...
	}

};
MyLog mylog;
int main(int argc, char* argv[])
{
	LogMgr::Obj().SetLogPrinter(&mylog)
}
	
*/
#pragma once
#include <string>

namespace su
{
enum LogLv
{
    //优先级从高到底
    LL_FATAL,
    LL_ERROR,
	LL_WARN,
	LL_INFO,
    LL_DEBUG,
    LL_TRACE //追踪BUG用，频繁打印的时候用
};

//用户重定义打印log接口
class ILogPrinter
{
public:
	virtual void Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp) = 0;
	virtual void flush(){};
	
};

//缺省定义,打印到文件和标准输出
class DefaultLog : public ILogPrinter
{
public:
	virtual void Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp) override;
	virtual void flush() override;
public:
	//para:const char *fname, 文件路径名
	explicit DefaultLog(const char *fname = "log.txt");
	~DefaultLog();
	void setShowLv(LogLv lv);
	//print log in std out.
	void setStdOut(bool is_std_out);
private:
	const char * GetLogLevelStr(LogLv lv) const;

private:
	LogLv m_log_lv;
	FILE *m_file;
	bool m_is_std_out;
	std::string m_prefix_name;
};


//单例
class LogMgr
{
public:
	static LogMgr &Obj()
	{
		static LogMgr d;
		return d;
	}
	void SetLogPrinter(ILogPrinter &iprinter); //改变日志实现
	void Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, ...) ;
	void Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp);
	void PrintfCond(LogLv lv, const char * file, int line, const char *fun, const char * cond, const char * pattern="", ...) ;
	void flush();

private:
	LogMgr();

private:
	DefaultLog m_log;
	ILogPrinter *m_iprinter;
};


}