#include "binding-util.h"
#include "exception.h"

#ifdef WIN32
#include <Windows.h>
#endif

MRB_FUNCTION(graphicsUpdate)
{
	MRB_FUN_UNUSED_PARAM;
#ifdef WIN32
	Sleep(1000/60);
#else
	sleep(1000/60);
#endif 
	return mrb_nil_value();
}

MRB_FUNCTION(graphicsFreeze)
{
	MRB_FUN_UNUSED_PARAM;


	return mrb_nil_value();
}

MRB_FUNCTION(graphicsTransition)
{
	return mrb_nil_value();
}

MRB_FUNCTION(graphicsFrameReset)
{

	return mrb_nil_value();
}

static int g_frame_rate=60;
static int g_frame_count;
static int g_fullscreen;
static int g_showcurrsor;

MRB_FUNCTION(graphicsGetFrameRate)
{
	return mrb_float_value(mrb,g_frame_rate);
}
MRB_FUNCTION(graphicsSetFrameRate)
{
	int rate;
	mrb_get_args(mrb,"i",&rate);
	g_frame_rate = rate;
	return mrb_nil_value();
}

MRB_FUNCTION(graphicsGetFrameCount)
{
	return mrb_float_value(mrb,g_frame_count);
}
MRB_FUNCTION(graphicsSetFrameCount)
{
	int count;
	mrb_get_args(mrb,"i",&count);
	g_frame_count = count;
	return mrb_nil_value();
}

MRB_FUNCTION(graphicsGetFullscreen)
{
	return mrb_fixnum_value(g_fullscreen);
}
MRB_FUNCTION(graphicsSetFullscreen)
{
	int full;
	mrb_get_args(mrb,"i",&full);
	g_fullscreen = full;
	return mrb_nil_value();
}

MRB_FUNCTION(graphicsGetShowCursor)
{
	return mrb_fixnum_value(g_showcurrsor);
}
MRB_FUNCTION(graphicsSetShowCursor)
{
	int showcursor;
	mrb_get_args(mrb,"i",&showcursor);
	g_showcurrsor = showcursor;
	return mrb_nil_value();
}

#define INIT_GRA_PROP_BIND(PropName, prop_name_s) \
{ \
	mrb_define_module_function(mrb, module, prop_name_s, graphics##Get##PropName, MRB_ARGS_NONE()); \
	mrb_define_module_function(mrb, module, prop_name_s "=", graphics##Set##PropName, MRB_ARGS_REQ(1)); \
}

void graphicsBindingInit(mrb_state *mrb)
{
	RClass *module = mrb_define_module(mrb, "Graphics");

	mrb_define_module_function(mrb, module, "update", graphicsUpdate, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module, "freeze", graphicsFreeze, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module, "transition", graphicsTransition, MRB_ARGS_OPT(3));
	mrb_define_module_function(mrb, module, "frame_reset", graphicsFrameReset, MRB_ARGS_NONE());

	INIT_GRA_PROP_BIND( FrameRate,  "frame_rate"  );
	INIT_GRA_PROP_BIND( FrameCount, "frame_count" );

	INIT_GRA_PROP_BIND( Fullscreen, "fullscreen"  );
	INIT_GRA_PROP_BIND( ShowCursor, "show_cursor" );
}
