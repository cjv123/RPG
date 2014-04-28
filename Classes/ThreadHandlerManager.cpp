#include "ThreadHandlerManager.h"

ThreadHandlerMananger* ThreadHandlerMananger::getInstance()
{
	static ThreadHandlerMananger instance;
	return &instance;
}

void ThreadHandlerMananger::pushHandler( ThreadHandler& handler ,void* instance)
{
	handler.instance = (int)instance;
	m_HandlerList.push_back(handler);
}

void ThreadHandlerMananger::pushHandlerAudio( ThreadHandler& handler )
{
	m_HandlerAudioList.push_back(handler);
}


void ThreadHandlerMananger::pushHandlerRelease( ThreadHandler& handler )
{
	m_handleReleaseList.push_back(handler);
}


void ThreadHandlerMananger::deleteHandler( int instance )
{
	list<ThreadHandler>::iterator it = m_HandlerList.begin();
	for(;it!=m_HandlerList.end();)
	{
		if (it->instance == instance)
		{
			it = m_HandlerList.erase(it);
		}
		else
			it++;
	}
}


void ThreadHandlerMananger::update( float delay )
{
	while (m_HandlerList.size()>0)
	{
		ThreadHandler& handler =  m_HandlerList.front();
		handler.method(handler.ptr1,handler.ptr2);
		m_HandlerList.pop_front();
	}

	while (m_HandlerAudioList.size()>0)
	{
		ThreadHandler& handler =  m_HandlerAudioList.front();
		handler.method(handler.ptr1,handler.ptr2);
		m_HandlerAudioList.pop_front();
	}

	while (m_handleReleaseList.size()>0)
	{
		ThreadHandler& handler = m_handleReleaseList.front();
		handler.method(handler.ptr1,handler.ptr2);
		m_handleReleaseList.pop_front();
	}
}

ThreadHandlerMananger::ThreadHandlerMananger()
{
	
}


