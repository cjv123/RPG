#ifndef _THREADHANDLERMANAGER_H
#define _THREADHANDLERMANAGER_H

#include <pthread.h>

#include <list>
#include "binding-mruby/disposable.h"
using namespace std;

typedef int (*THREAD_HANDLER_FUNC)(int,void*);   

struct ThreadHandler
{
	THREAD_HANDLER_FUNC method;
	int ptr1;
	void* ptr2;
	int instance;
};

class ThreadHandlerMananger
{
public:
	static ThreadHandlerMananger* getInstance();
	void pushHandler(ThreadHandler& handler,void* instance);
	void pushHandlerAudio(ThreadHandler& handler);
	void pushHandlerRelease(ThreadHandler& handler);
	void deleteHandler(int insance);
	void update(float delay);
protected:
	list<ThreadHandler> m_HandlerList;
	list<ThreadHandler> m_HandlerAudioList;
	list<ThreadHandler> m_handleReleaseList;
	ThreadHandlerMananger();
};

#endif
