#include "binding-util.h"
#include "exception.h"
#include "SimpleAudioEngine.h"
#include "..\ThreadHandlerManager.h"
using namespace CocosDenshion;

static string getSoundFilename(const char* path)
{
	string filename = string(path) + ".mid";
	unsigned long size=0;
	unsigned char* data =CCFileUtils::sharedFileUtils()->getFileData(filename.c_str(),"rb",&size);
	if (size)
	{
		delete [] data;
		return filename;
	}

	filename = string(path) + ".ogg";
	data =CCFileUtils::sharedFileUtils()->getFileData(filename.c_str(),"rb",&size);
	if (size)
	{
		delete [] data;
		return filename;
	}

	filename = string(path) + ".mp3";
	data =CCFileUtils::sharedFileUtils()->getFileData(filename.c_str(),"rb",&size);
	if (size)
	{
		delete [] data;
		return filename;
	}
	
}

static int g_gbs_id = 0;
static int g_se_id = 0;
static int g_me_id = 0;

int handler_method_play_audio(int ptr1,void* ptr2) 
{
	int type = ptr1;
	string* path = (string*)ptr2;
	switch (type)
	{
	case 1:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(path->c_str(),true);
		break;
	case 2:
		if (g_gbs_id)
			SimpleAudioEngine::sharedEngine()->stopEffect(g_gbs_id);
		g_gbs_id =SimpleAudioEngine::sharedEngine()->playEffect(path->c_str(),true);
		break;
	case 3:
		if (g_se_id)
			SimpleAudioEngine::sharedEngine()->stopEffect(g_se_id);
		g_se_id =SimpleAudioEngine::sharedEngine()->playEffect(path->c_str());
		break;
	case 4:
		if (g_me_id)
			SimpleAudioEngine::sharedEngine()->stopEffect(g_me_id);
		g_me_id =SimpleAudioEngine::sharedEngine()->playEffect(path->c_str());
		break;
	}

	delete path;
	return 0;
}

int handler_method_stop_audio(int ptr1,void* ptr2) 
{
	int type = ptr1;
	switch (type)
	{
	case 1:
		if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		break;
	case 2:
		if(g_gbs_id)
		{
			SimpleAudioEngine::sharedEngine()->stopEffect(g_gbs_id);
			g_gbs_id = 0;
		}
		break;
	case 3:
		if (g_se_id)
		{
			SimpleAudioEngine::sharedEngine()->stopEffect(g_se_id);
			g_se_id = 0;
		}
		break;
	case 4:
		if (g_me_id)
		{
			SimpleAudioEngine::sharedEngine()->stopEffect(g_me_id);
			g_me_id = 0;
		}
		break;
	}
	return 0;
}

extern pthread_mutex_t s_thread_handler_mutex;
static void play_bgm_sound(const char* path)
{
	string* filename =new string(getSoundFilename(path));
	ThreadHandler hander={handler_method_play_audio,1,(void*)filename};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

static void play_bgs_sound(const char* path)
{
	string* filename =new string(getSoundFilename(path));
	ThreadHandler hander={handler_method_play_audio,2,(void*)filename};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

static void play_se_sound(const char* path)
{
	string* filename =new string(getSoundFilename(path));
	ThreadHandler hander={handler_method_play_audio,3,(void*)filename};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

static void play_me_sound(const char* path)
{
	string* filename =new string(getSoundFilename(path));
	ThreadHandler hander={handler_method_play_audio,4,(void*)filename};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

static void stop_bgm_sound()
{
	ThreadHandler hander={handler_method_stop_audio,1,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

static void stop_bgs_sound()
{
	ThreadHandler hander={handler_method_stop_audio,2,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

static void stop_se_sound()
{
	ThreadHandler hander={handler_method_stop_audio,3,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

static void stop_me_sound()
{
	ThreadHandler hander={handler_method_stop_audio,4,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerAudio(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

/*
play_##entity##_sound(filename); \
	stop_##entity##_sound(); \*/

#define DEF_PLAY_STOP(entity) \
	MRB_FUNCTION(audio_##entity##Play) \
	{ \
		char *filename; \
		mrb_int volume = 100; \
		mrb_int pitch = 100; \
		mrb_get_args(mrb, "z|ii", &filename, &volume, &pitch); \
		play_##entity##_sound(filename); \
		return mrb_nil_value(); \
	} \
	MRB_FUNCTION(audio_##entity##Stop) \
	{ \
		MRB_FUN_UNUSED_PARAM; \
		stop_##entity##_sound(); \
		return mrb_nil_value(); \
	}

#define DEF_FADE(entity) \
MRB_FUNCTION(audio_##entity##Fade) \
{ \
	mrb_int time; \
	mrb_get_args(mrb, "i", &time); \
	return mrb_nil_value(); \
}

#define DEF_PLAY_STOP_FADE(entity) \
	DEF_PLAY_STOP(entity) \
	DEF_FADE(entity)

DEF_PLAY_STOP_FADE( bgm )
DEF_PLAY_STOP_FADE( bgs )
DEF_PLAY_STOP_FADE( me  )

DEF_PLAY_STOP( se  )


#define BIND_PLAY_STOP(entity) \
	mrb_define_module_function(mrb, module, #entity "_play", audio_##entity##Play, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2)); \
	mrb_define_module_function(mrb, module, #entity "_stop", audio_##entity##Stop, MRB_ARGS_NONE());

#define BIND_FADE(entity) \
	mrb_define_module_function(mrb, module, #entity "_fade", audio_##entity##Fade, MRB_ARGS_REQ(1));

#define BIND_PLAY_STOP_FADE(entity) \
	BIND_PLAY_STOP(entity) \
	BIND_FADE(entity)


void audioBindingInit(mrb_state *mrb)
{
	RClass *module = mrb_define_module(mrb, "Audio");

	BIND_PLAY_STOP_FADE( bgm )
	BIND_PLAY_STOP_FADE( bgs )
	BIND_PLAY_STOP_FADE( me  )

	BIND_PLAY_STOP( se )
}
