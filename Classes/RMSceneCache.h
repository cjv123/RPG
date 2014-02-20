#ifndef _RMSCENECACHE_H
#define _RMSCENECACHE_H

#include <cocos2d.h>
using namespace cocos2d;

#include <pthread.h>

extern pthread_mutex_t  s_draw_cache_mutex;

class RMSceneCache : public CCRenderTexture
{
public:
	 static RMSceneCache* getInstance();

	 virtual bool init(int w, int h);

	 virtual void visit();

	 
private:

};

#endif

