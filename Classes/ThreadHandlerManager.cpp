#include "ThreadHandlerManager.h"

ThreadHandlerMananger* ThreadHandlerMananger::getInstance()
{
	static ThreadHandlerMananger instance;
	return &instance;
}

void ThreadHandlerMananger::pushHandler( ThreadHandler& handler )
{
	m_HandlerList.push_back(handler);
}

void ThreadHandlerMananger::update( float delay )
{
	int handlerSize = m_HandlerList.size();
	if (handlerSize>0)
	{
		for (int i=0;i<handlerSize;i++)
		{
			ThreadHandler& handler =  m_HandlerList.front();
			handler.method(handler.ptr1,handler.ptr2);
			m_HandlerList.pop_front();
		}
	}
}

ThreadHandlerMananger::ThreadHandlerMananger()
{
	
}

