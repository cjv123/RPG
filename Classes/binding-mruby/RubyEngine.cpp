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

extern void etcBindingInit(mrb_state *mrb);
extern void fontBindingInit(mrb_state* mrb);
extern void bitmapBindingInit(mrb_state *mrb);
extern void spriteBindingInit(mrb_state *mrb);

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

extern char module_rpg[];
extern unsigned int module_rpg_len;

mrb_state* RubyEngine::initRubyEngine()
{
	m_mrb= mrb_open();

	static MrbData mrbData(m_mrb);
	m_mrb->ud = &mrbData;

	return m_mrb;
}

void RubyEngine::initBindingMethod()
{
	mrb_mruby_marshal_gem_init(m_mrb);
	kernelBindingInit(m_mrb);

	etcBindingInit(m_mrb);
	fontBindingInit(m_mrb);
	bitmapBindingInit(m_mrb);
	spriteBindingInit(m_mrb);
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

	char msgBoxText[512];
	snprintf(msgBoxText, 512, "Script '%s' line %d: %s occured.\n\n%s",
		mrbValueString(file), mrb_fixnum(line), excClass, mrbValueString(mesg));

	printf(msgBoxText);
}

void RubyEngine::runScript(const char* script )
{
	int len = strlen(script);

	mrbc_context *ctx = mrbc_context_new(m_mrb);
	ctx->capture_errors = 1;
	mrb_load_nstring_cxt(m_mrb, script, len, ctx);
	//checkException();
}

void RubyEngine::initRMXPScript( const char* filename )
{
	unsigned long size;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(filename,"rb",&size);
	mrb_value mrb_arr = mrb_marshal_load(m_mrb,(char*)data,size);
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
	}
	delete [] data;
}

static pthread_t s_networkThread;
pthread_mutex_t s_thread_handler_mutex;

void RubyEngine::runRMXPScript()
{
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
		engine->runScript(engine->m_RMXPScripts[i].script.c_str());
	}
	engine->checkException();

	mrb_close(engine->m_mrb);
	pthread_exit(0);

	pthread_mutex_destroy(&s_thread_handler_mutex);

	return 0;
}
