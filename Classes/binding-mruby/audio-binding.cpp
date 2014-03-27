#include "binding-util.h"
#include "exception.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

static string getSoundFilename(const char* path)
{
	string filename = string(path) + ".mid";
	FILE* fp = fopen(filename.c_str(),"rb");
	if (fp)
	{
		fclose(fp);
		return filename;
	}

	filename = string(path) + ".ogg";
	fp = fopen(filename.c_str(),"rb");
	if (fp)
	{
		fclose(fp);
		return filename;
	}

	filename = string(path) + ".mp3";
	fp = fopen(filename.c_str(),"rb");
	if (fp)
	{
		fclose(fp);
		return filename;
	}
	
}

static int g_gbs_id = 0;
static int g_se_id = 0;
static int g_me_id = 0;

static void play_bgm_sound(const char* path)
{
	string filename = getSoundFilename(path);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(filename.c_str(),true);
}

static void play_bgs_sound(const char* path)
{
	string filename = getSoundFilename(path);
	if (g_gbs_id)
		SimpleAudioEngine::sharedEngine()->stopEffect(g_gbs_id);
	g_gbs_id =SimpleAudioEngine::sharedEngine()->playEffect(filename.c_str(),true);
}

static void play_se_sound(const char* path)
{
	string filename = getSoundFilename(path);
	if (g_se_id)
		SimpleAudioEngine::sharedEngine()->stopEffect(g_se_id);
	g_se_id = SimpleAudioEngine::sharedEngine()->playEffect(filename.c_str());
}

static void play_me_sound(const char* path)
{
	string filename = getSoundFilename(path);
	if (g_me_id)
		SimpleAudioEngine::sharedEngine()->stopEffect(g_me_id);
	g_me_id = SimpleAudioEngine::sharedEngine()->playEffect(filename.c_str());
}

static void stop_bgm_sound()
{
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

static void stop_bgs_sound()
{
	if(g_gbs_id)
		SimpleAudioEngine::sharedEngine()->stopEffect(g_gbs_id);
}

static void stop_se_sound()
{
	if (g_se_id)
			SimpleAudioEngine::sharedEngine()->stopEffect(g_se_id);
}

static void stop_me_sound()
{
	if (g_me_id)
		SimpleAudioEngine::sharedEngine()->stopEffect(g_me_id);
}


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
