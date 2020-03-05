#include "concurrency/swap_queue.h"
#include <algorithm>
#include <cstring>

using namespace std;

SwapQueue::SwapQueue()
{
	m_pwrite = &m_queue_a;
	m_pread = &m_queue_b;
}

void SwapQueue::Write(const char *data, uint32 len)
{
	m_pwrite->append(data, len);
	OnTimer();
}

void SwapQueue::OnTimer()
{
	//if (C == false && W 有数据)  swap(R, W, ) C = true
	if (!m_pwrite->empty() && false == m_is_reading)
	{
		swap(m_pwrite, m_pread);
		m_read_pos = 0;
		m_pwrite->clear();
		m_is_reading = true;//必须放最后，读线程看到true就开始去读取了
	}
}

bool SwapQueue::IsNeedTimer()
{
	return !m_pwrite->empty();
}

void SwapQueue::Read(char *data, uint32 &len)
{
	//if (C == false) idle
	//else(C == true) read
	//if (have read all R) C = false
	if (false == m_is_reading)
	{
		len = 0;
		return;
	}

	//read data
	uint32 remain_len = m_pread->size() - m_read_pos;
	len = std::min(len, remain_len);
	memcpy(data, &((*m_pread)[m_read_pos]), len);
	m_read_pos += len;

	if (m_read_pos>= m_pread->size())
	{
		m_is_reading = false;
		return;
	}
}
