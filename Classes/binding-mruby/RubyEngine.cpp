#include "RubyEngine.h"
#include "kernel.h"
#include "marshal.h"

#include <string.h>
#include <mruby/value.h>
#include <mruby/string.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <cocos2d.h>
using namespace cocos2d;

#include <zlib.h>
#include <pthread.h>

#ifdef WIN32
#include <Windows.h>
#endif

extern void etcBindingInit(mrb_state *mrb);
extern void tableBindingInit(mrb_state *mrb);
extern void fontBindingInit(mrb_state* mrb);
extern void bitmapBindingInit(mrb_state *mrb);
extern void spriteBindingInit(mrb_state *mrb);
extern void graphicsBindingInit(mrb_state *mrb);
extern void viewportBindingInit(mrb_state *mrb);
extern void tilemapBindingInit(mrb_state *mrb);
extern void windowBindingInit(mrb_state *mrb);
extern void inputBindingInit(mrb_state *mrb);
extern void audioBindingInit(mrb_state* mrb);
extern void timeBindingInit(mrb_state* mrb);
extern void fileBindingInit(mrb_state* mrb);
extern void inputBindingInit(mrb_state* mrb);
extern void planeBindingInit(mrb_state *mrb);
extern void marshalBindingInit(mrb_state *mrb);

static const char * mrbValueString(mrb_value value)
{
	return mrb_string_p(value) ? RSTRING_PTR(value) : 0;
}

mrb_state* RubyEngine::initAll()
{
	mrb_state* mrb = initRubyEngine();
	initBindingMethod();

	return mrb; 
}

extern uint8_t mrbModuleRPG[];

mrb_state* RubyEngine::initRubyEngine()
{
	m_mrb= mrb_open();

	static MrbData mrbData(m_mrb);
	m_mrb->ud = &mrbData;

	return m_mrb;
}

void RubyEngine::initBindingMethod()
{
	int arena = mrb_gc_arena_save(m_mrb);

	//mrb_mruby_marshal_gem_init(m_mrb);
	marshalBindingInit(m_mrb);
	kernelBindingInit(m_mrb);
	etcBindingInit(m_mrb);
	tableBindingInit(m_mrb);
	graphicsBindingInit(m_mrb);
	fontBindingInit(m_mrb);
	bitmapBindingInit(m_mrb);
	spriteBindingInit(m_mrb);
	viewportBindingInit(m_mrb);
	tilemapBindingInit(m_mrb);
	windowBindingInit(m_mrb);
	inputBindingInit(m_mrb);
	audioBindingInit(m_mrb);
	timeBindingInit(m_mrb);
	fileBindingInit(m_mrb);
	inputBindingInit(m_mrb);
	planeBindingInit(m_mrb);

	//mrb_load_irep(m_mrb, mrbModuleRPG);
	unsigned long size;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData("rpg.rb","rb",&size);
	runScript((const char*)data,size);
	delete [] data;
	mrb_define_global_const(m_mrb, "MKXP", mrb_true_value());

	mrb_gc_arena_restore(m_mrb, arena);
}

mrb_state* RubyEngine::getMrbState()
{
	return m_mrb;
}

void RubyEngine::checkException()
{
	if (!m_mrb->exc)
		return;

	mrb_value exc = mrb_obj_value(m_mrb->exc);
	MrbData &mrbData = *getMrbData(m_mrb);

	/* Check if an actual exception occured, or just a shutdown was called */
	if (mrb_obj_class(m_mrb, exc) != mrbData.exc[Shutdown])
		showExcMessageBox(exc);
}

void RubyEngine::showExcMessageBox(mrb_value exc)
{
	mrb_value mesg = mrb_funcall(m_mrb, exc, "message", 0);
	mrb_value line = mrb_attr_get(m_mrb, exc, mrb_intern_lit(m_mrb, "line"));
	mrb_value file = mrb_attr_get(m_mrb, exc, mrb_intern_lit(m_mrb, "file"));
	const char *excClass = mrb_class_name(m_mrb, mrb_class(m_mrb, exc));

	CCLOG("Script '%s' line %d: %s occured.\n%s\n",
		mrbValueString(file), mrb_fixnum(line), excClass, mrbValueString(mesg));
}

void RubyEngine::runScript(const char* script,int len/*=0*/ )
{
	int s = len;
	if(s==0)
		s = strlen(script);

	mrbc_context *ctx = mrbc_context_new(m_mrb);
	ctx->capture_errors = 1;
	mrb_load_nstring_cxt(m_mrb, script, s, ctx);
	//checkException();
}

void RubyEngine::initRMXPScript( const char* filename )
{
	unsigned long size;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(filename,"rb",&size);
	mrb_value mrb_arr = marshalLoadInt(m_mrb,(char*)data,size);
	int arrlen = RARRAY_LEN(mrb_arr);
	for (int i=0;i<arrlen;i++)
	{
		mrb_value mrb_script_arr = mrb_ary_entry(mrb_arr, i);
		int id = mrb_fixnum(mrb_ary_entry(mrb_script_arr, 0));
		char* scriptname = mrb_string_value_ptr(m_mrb,mrb_ary_entry(mrb_script_arr, 1));
		mrb_value scriptString= mrb_ary_entry(mrb_script_arr, 2);

		int result = Z_OK;
		std::string decodeBuffer;
		decodeBuffer.resize(0x1000);
		unsigned long bufferLen;
		while (true)
		{
			unsigned char *bufferPtr =
				reinterpret_cast<unsigned char*>(const_cast<char*>(decodeBuffer.c_str()));
			unsigned char *sourcePtr =
				reinterpret_cast<unsigned char*>(RSTRING_PTR(scriptString));

			bufferLen = decodeBuffer.length();

			result = uncompress(bufferPtr, &bufferLen,
				sourcePtr, RSTRING_LEN(scriptString));

			bufferPtr[bufferLen] = '\0';

			if (result != Z_BUF_ERROR)
				break;

			decodeBuffer.resize(decodeBuffer.size()*2);
		}

		if (result != Z_OK)
		{
			static char buffer[256];
			snprintf(buffer, sizeof(buffer), "Error decoding script %d: '%s'",
				i, scriptname);

			printf("%s\n",buffer);

			break;
		}
		
		struct RMXPScript scriptdata={id,scriptname,decodeBuffer};
		m_RMXPScripts.push_back(scriptdata);

// 		char header[3] = {0xef, 0xbb, 0xbf};
// 		if (scriptdata.name!="")
// 		{
// 			string path = scriptdata.name + ".rb";
// 			FILE* f = fopen(path.c_str(),"wb");
// 			fwrite(header,sizeof(char),3,f);
// 			fwrite(decodeBuffer.c_str(),sizeof(char),decodeBuffer.size(),f);
// 			fclose(f);
// 		}
		
	}

	delete [] data;

	//mruby的语法可能不兼容的地方 只能替换一下写法
	for (int i=0;i<m_RMXPScripts.size();i++)
	{
		if (m_RMXPScripts[i].name == "Scene_Battle 3")
		{
			string& script = m_RMXPScripts[i].script;
			string findstr="end until @active_battler.inputable?";
			int pos = script.find(findstr.c_str());
			if (pos!=-1)
				script.replace(pos,strlen(findstr.c_str()),"break if @active_battler.inputable?\nend until false");

			findstr="unless $game_party.item_can_use?(@item.id)";
			pos = script.find(findstr.c_str());
			if (pos!=-1)
				script.replace(pos,strlen(findstr.c_str()),"unless @item!=nil and $game_party.item_can_use?(@item.id)");

			return;
		}

		if (m_RMXPScripts[i].name == "Window_Message")
		{
			string& script = m_RMXPScripts[i].script;
			string findstr="$1";
			while(1)
			{
				int pos = script.find(findstr.c_str());
				if (pos!=-1)
					script.replace(pos,strlen(findstr.c_str()),"Regexp.last_match[1]");
				else
					break;
			}
		}
	}
}

static pthread_t s_networkThread;
pthread_mutex_t s_thread_handler_mutex;

void RubyEngine::runRMXPScript()
{
	m_runRMXP = true;
	pthread_mutex_init(&s_thread_handler_mutex, NULL);
	pthread_create(&s_networkThread, NULL, networkThread, (void*)this);
	pthread_detach(s_networkThread);
}

void* RubyEngine::networkThread( void* data )
{
	RubyEngine* engine = (RubyEngine*)data;

	int script_cout = engine->m_RMXPScripts.size();
	for (int i=0;i<script_cout;i++)
	{
		int ai = mrb_gc_arena_save(engine->m_mrb);
		CCLOG("run script:%s",engine->m_RMXPScripts[i].name.c_str());
		engine->runScript(engine->m_RMXPScripts[i].script.c_str());
		mrb_gc_arena_restore(engine->m_mrb, ai);
		engine->checkException();
	}

#ifndef WIN32
	CCDirector::sharedDirector()->end();
#endif
	pthread_exit(0);
	pthread_mutex_destroy(&s_thread_handler_mutex);

	return 0;
}

RubyEngine::RubyEngine() : m_runRMXP(false)
{

}


RubyEngine::~RubyEngine()
{
	mrb_close(m_mrb);
}

bool RubyEngine::getRunRMXP()
{
	return m_runRMXP;
}

