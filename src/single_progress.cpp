#include "single_progress.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/param.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include "log_def.h"
#include "time/su_timer.h"

using namespace std;
using namespace su;

file_lock::file_lock(const std::string& file_name)
:m_fd(-1)
{
	m_pid_file = "./" + file_name + ".lock";
}

file_lock::~file_lock()
{
	unlock();
}

//���Ը� �ļ���ӵ�н��̷��ź�
int file_lock::kill(int sig)
{
	int fd = open(m_pid_file.c_str(), O_TRUNC | O_RDWR);
	if (fd < 0)
	{
		L_ERROR("open fail fail, open fd = %d, %d\n", fd, sig);
		return -1;
	}

	flock flock_data;
	flock_data.l_type = F_WRLCK;
	flock_data.l_start = 0;
	flock_data.l_whence = SEEK_SET;
	flock_data.l_len = 0;
	//�ж������ļ��Ƿ���д����F_WRLCK��
	if (fcntl(fd, F_GETLK, reinterpret_cast<long>(&flock_data)) == -1)
	{
		L_ERROR("fcntl error, fd = %d, %d\n", fd, sig);
		return -1;
	}

	if (flock_data.l_type == F_UNLCK)//case no lock
	{
		L_ERROR("file no lock");
		return 0;
	}
	//ע�⣬������̲���ԭ�Ӳ����������п�����״̬�仯~��

	//���������������������Ϣ�Ѿ�д�� flock_data
	// if (::kill(lock.l_pid, sig) == -1)
	// ������ʱд����Kill, unblockable (POSIX).
	if (::kill(flock_data.l_pid, sig) == -1)
	{
		L_ERROR("file_lock::kill ::kill(lock.l_pid, sig) == -1, pid = %d, sig = %d, errno = %s, %d", flock_data.l_pid, sig, strerror(errno), errno);
		return -1;
	}
	else
	{
		return 1;
	}
}

bool file_lock::lock()
{
	m_fd = open(m_pid_file.c_str(), O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (m_fd < 0)
	{
		L_ERROR("base -> dispatch_events daemon failure! m_fd = %d,%s(%d)", m_fd, strerror(errno), errno);
		return false;
	}
	int lock_result = lockf(m_fd, F_TEST, 0);
	if (lock_result < 0)
	{
		// lockf F_TEST failure.
		//L_ERROR("base -> dispatch_events daemon failure!F_TEST lock_result = %d", lock_result);
		return false;
	}
	lock_result = lockf(m_fd, F_LOCK, 0);
	if (lock_result < 0)
	{
		// lockf F_LOCK failure.
		L_ERROR("base -> dispatch_events daemon failure!F_LOCK lock_result = %d", lock_result);
		return false;
	}
	return true;
}

void file_lock::unlock()
{
	if (-1 == m_fd )
	{
		return;
	}
	lockf(m_fd, F_ULOCK, 0);
}

SingleProgress::SingleProgress()
:m_is_exit(false)
, m_old_cb(nullptr)
{

}


void SingleProgress::CheckSingle(const std::string &single_file_name)
{
	static file_lock m_file_lock(single_file_name);
	if (!m_file_lock.lock())
	{
		L_DEBUG("progress %s aleady run!", single_file_name.c_str());
		exit(1);
	}
	sighandler_t old_cb = signal(SIGUSR1, SingleProgress::catch_signal);
	//SIG_IGN ���θ��ź�        �� SIG_DFL �ָ�Ĭ����
	if ((old_cb != SIG_IGN)
		&&
		(old_cb != SIG_DFL)
		)
	{
		m_old_cb = old_cb;
	}
}

void SingleProgress::StopSingle(const std::string &single_file_name)
{
	static file_lock m_file_lock(single_file_name);
	if (!m_file_lock.kill(SIGUSR1))
	{
		L_DEBUG("can't find runing progress to stop");
		return;
	}
}

void SingleProgress::catch_signal(int sig_type)
{
	SingleProgress::Obj().m_is_exit = true;
	if (nullptr != SingleProgress::Obj().m_old_cb)
	{
		SingleProgress::Obj().m_old_cb(sig_type);
	}
}


void SPMgr::Stop(const char *pname)
{
	L_COND(pname);
	SingleProgress::Obj().StopSingle(pname);
	exit(1);
}


void SPMgr::Check(int argc, char* argv[], const char *pname, ExitProccessCB cb)
{
	//start or stop proccess
	if (argc == 2 && string("stop") == argv[1])
	{
		SPMgr::Obj().Stop(pname);
		return;
	}
	SPMgr::Obj().Start(pname, cb);
}

void SPMgr::Start(const char *pname, ExitProccessCB cb)
{
	L_COND(pname);
	SingleProgress::Obj().CheckSingle(pname);
	m_cb = cb;
	auto f = std::bind(&SPMgr::CheckStopProccess, this);
	m_tm.StartTimer(1, f, true); 
}

void SPMgr::CheckStopProccess()
{
	if (!SingleProgress::Obj().IsExit())
	{
		return;
	}
	if (m_cb)
	{
		m_cb();
	}
	L_DEBUG("==============end proccess===========");
	exit(1);
}
