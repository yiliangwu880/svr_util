#include "game_util/post_fun.h"

void su::PostFunMgr::Post(PostFun f)
{
	m_vec.push_back(f);
}

void su::PostFunMgr::Excute()
{
	if (m_vec.empty())//�����ᶼ�ǿգ�������Ч��
	{
		return;
	}
	m_tmpVec.swap(m_vec); //���ƣ���֪���ص������᲻���޸� m_vec��
	for (PostFun &v : m_tmpVec)
	{
		v();
	}
	m_tmpVec.clear();
}
