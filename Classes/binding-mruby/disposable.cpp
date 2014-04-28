#include "disposable.h"
#include "../ThreadHandlerManager.h"

extern pthread_mutex_t s_thread_handler_mutex;

void Disposable::dispose()
{
	if (disposed)
		return;

	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->deleteHandler((int)this);
	pthread_mutex_unlock(&s_thread_handler_mutex);

	releaseResources();
	disposed = true;

}
