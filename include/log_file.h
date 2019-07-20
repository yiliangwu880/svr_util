/*
	д��ʱ��Ҫ��ӡ��־��ͬʱ��ʹ�ÿ���û��ṩ�ı���־ʵ�ֵ�ѡ��
	��־���� log_def.h.  ��������ʱ��һ�㲻���û���log_def.h�� �û��Լ������µĺ��������µ�ʵ��

�û��ı���־ʵ�����ӣ�
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

namespace svr_util
{
enum LogLv
{
    //���ȼ��Ӹߵ���
    LL_FATAL,
    LL_ERROR,
    LL_WARN,
    LL_DEBUG,
    //����ļ��𣬲�����ļ�λ����Ϣ
    LL_INFO,
    LL_ANY
};

//�û��ض����ӡlog�ӿ�
class ILogPrinter
{
public:
	virtual void Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp) = 0;
	
};

//ȱʡ����,��ӡ���ļ��ͱ�׼���
class DefaultLog : public ILogPrinter
{
public:
	virtual void Printf(LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp) override;
public:
	//para:const char *fname, �ļ�·����
	explicit DefaultLog(const char *fname = "log.txt");
	~DefaultLog();
	void setShowLv(LogLv lv);
	//print log in std out.
	void setStdOut(bool is_std_out);
	void flush();
private:
	const char * GetLogLevelStr(LogLv lv) const;

private:
	LogLv m_log_lv;
	FILE *m_file;
	bool m_is_std_out;
	std::string m_prefix_name;
};


//����
class LogMgr
{
public:
	static LogMgr &Obj()
	{
		static LogMgr d;
		return d;
	}
	void SetLogPrinter(ILogPrinter &iprinter); //�ı���־ʵ��
	void Printf(LogLv lv, const char * file, int line, const char *pFun, const char * pattern, ...) ;

private:
	LogMgr();

private:
	DefaultLog m_log;
	ILogPrinter *m_iprinter;
};


}