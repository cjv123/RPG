#include "binding-util.h"
#include "exception.h"

#define DEF_PLAY_STOP(entity) \
	MRB_FUNCTION(audio_##entity##Play) \
	{ \
		char *filename; \
		mrb_int volume = 100; \
		mrb_int pitch = 100; \
		mrb_get_args(mrb, "z|ii", &filename, &volume, &pitch); \
		return mrb_nil_value(); \
	} \
	MRB_FUNCTION(audio_##entity##Stop) \
	{ \
		MRB_FUN_UNUSED_PARAM; \
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
