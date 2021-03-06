#include "game_util/publish_subscribe.h"

void su::PostEvent::Add(const PostEventFun &fun)
{
	m_add_event->push_back(fun);
}

void su::PostEvent::Do()
{
	std::swap(m_add_event, m_do_event);
	for (const auto &v : *m_do_event)
	{
		v();
	}
	m_do_event->clear();
}



