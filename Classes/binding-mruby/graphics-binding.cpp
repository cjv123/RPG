#include "binding-util.h"
#include "exception.h"

#include <string>
using namespace std;
#include "ThreadHandlerManager.h"
#include <cocos2d.h>
using namespace cocos2d;
#include "SceneMain.h"

#ifdef WIN32
#include <Windows.h>
#endif


int g_frame_rate=60;
static unsigned long g_frame_count=0;
static const unsigned long g_frame_count_max = 0xffffffffffffffff;
static int g_fullscreen;
static int g_showcurrsor;

static int g_stop = 0;

static CCLayerColor* g_fide_layer=NULL;

static void graphic_sleep(float delay)
{
#ifdef WIN32
	Sleep(delay);
#else
	usleep(delay*1000);
#endif 
}

MRB_FUNCTION(graphicsUpdate)
{
	MRB_FUN_UNUSED_PARAM;

	if (g_stop)
	{
		while(g_stop)
		{
			graphic_sleep(10.0f);
		}
	}
	else
	{
		graphic_sleep(1000.0f/g_frame_rate);
		if (g_frame_count < g_frame_count_max)
			g_frame_count++;
	}
	return mrb_nil_value();
}

static int handler_method_freeze(int ptr1,void* ptr2)
{
	if (g_fide_layer==NULL)
	{
		g_fide_layer = CCLayerColor::create(ccc4(0,0,0,255));
		g_fide_layer->setContentSize(SceneMain::getMainLayer()->getContentSize());
		SceneMain::getMainLayer()->addChild(g_fide_layer,5000);
		CCSequence* seq = CCSequence::create(CCFadeIn::create(0.1f),NULL);
		g_fide_layer->runAction(seq);
	}
	return 0;
}

extern pthread_mutex_t s_thread_handler_mutex;

MRB_FUNCTION(graphicsFreeze)
{
	MRB_FUN_UNUSED_PARAM;
	g_stop = 1;

	ThreadHandler hander={handler_method_freeze,(int)0,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,NULL);
	pthread_mutex_unlock(&s_thread_handler_mutex);

	return mrb_nil_value();
}

struct TranstionPtrs 
{
	int duration;
	string filename;
	int vague;
};

class TransCallBack : public CCObject
{
public:
	void onTransComplete()
	{
		g_stop = 0;
	}
};

static int handler_method_graphicsTransition(int ptr1,void* ptr2)
{
	TranstionPtrs* ptr = (TranstionPtrs*)ptr1;

	if (ptr->filename=="")
	{
		if(g_fide_layer)
		{
			float delay = ptr->duration*1.0f/g_frame_rate;
			TransCallBack* callback = new TransCallBack;
			CCSequence* seq = CCSequence::create(CCFadeOut::create(delay),CCCallFunc::create(callback,callfunc_selector(TransCallBack::onTransComplete)),CCRemoveSelf::create(),NULL);
			g_fide_layer->runAction(seq);
		}
	}
	else
	{
		if(g_fide_layer)
			g_fide_layer->removeFromParentAndCleanup(true);

		string path = ptr->filename + ".png";
		CCSprite* sp = CCSprite::create(path.c_str());
		if (!sp)
			return -1;
		sp->setAnchorPoint(ccp(0,0));
		SceneMain::getMainLayer()->addChild(sp,5000);
		float delay = ptr->duration*1.0f/g_frame_rate;
		TransCallBack* callback = new TransCallBack;
		CCSequence* seq = CCSequence::create(CCFadeOut::create(delay),CCCallFunc::create(callback,callfunc_selector(TransCallBack::onTransComplete)),CCRemoveSelf::create(),NULL);
		sp->runAction(seq);
	}
	g_fide_layer = NULL;
	delete ptr;
	return 0;
}

MRB_FUNCTION(graphicsTransition)
{
	mrb_int duration=8,vague=40;
	char* filename=NULL;

	if (mrb->c->ci->argc==1)
		mrb_get_args(mrb, "i", &duration);
	else if (mrb->c->ci->argc == 2)
		mrb_get_args(mrb, "iz", &duration,&filename);
	else if (mrb->c->ci->argc == 3)
		mrb_get_args(mrb, "izi", &duration,&filename,&vague);
	

	TranstionPtrs* ptr = new TranstionPtrs;
	ptr->duration = duration;
	if (filename)
		ptr->filename = filename;
	ptr->vague = vague;

	ThreadHandler hander={handler_method_graphicsTransition,(int)ptr,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,NULL);
	pthread_mutex_unlock(&s_thread_handler_mutex);

	return mrb_nil_value();
}

MRB_FUNCTION(graphicsFrameReset)
{

	return mrb_nil_value();
}


MRB_FUNCTION(graphicsGetFrameRate)
{
	return mrb_fixnum_value(g_frame_rate);
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
	return mrb_fixnum_value(g_frame_count);
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
