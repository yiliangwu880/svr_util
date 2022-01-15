#include "game_util/post_fun.h"

void su::PostFunMgr::Post(PostFun f)
{
	m_vec.push_back(f);
}

void su::PostFunMgr::Excute()
{
	if (m_vec.empty())//大多机会都是空，提升点效率
	{
		return;
	}
	m_tmpVec.swap(m_vec); //复制，天知道回调函数会不会修改 m_vec！
	for (PostFun &v : m_tmpVec)
	{
		v();
	}
	m_tmpVec.clear();
}
