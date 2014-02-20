#ifndef _THREADHANDLERMANAGER_H
#define _THREADHANDLERMANAGER_H

#include <pthread.h>

#include <list>
using namespace std;

typedef int (*THREAD_HANDLER_FUNC)(int,void*);   

struct ThreadHandler
{
	THREAD_HANDLER_FUNC method;
	int ptr1;
	void* ptr2;
};

class ThreadHandlerMananger
{
public:
	static ThreadHandlerMananger* getInstance();
	void pushHandler(ThreadHandler& handler);
	void update(float delay);
protected:
	list<ThreadHandler> m_HandlerList;
	ThreadHandlerMananger();
};

#endif
