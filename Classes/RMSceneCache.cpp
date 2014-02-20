#include "RMSceneCache.h"

pthread_mutex_t  s_draw_cache_mutex;

bool  RMSceneCache::init( int w, int h )
{
	if(initWithWidthAndHeight(w, h, kCCTexture2DPixelFormat_RGBA8888, 0))
	{
		pthread_mutex_init(&s_draw_cache_mutex, NULL);
		return true;
	}

	return false;
}

void RMSceneCache::visit()
{
	pthread_mutex_lock(&s_draw_cache_mutex);
	CCRenderTexture::visit();
	pthread_mutex_unlock(&s_draw_cache_mutex);
}

RMSceneCache* RMSceneCache::getInstance()
{
	static RMSceneCache instance;
	return &instance;
}

