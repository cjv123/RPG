#include "SceneMain.h"
#include "ThreadHandlerManager.h"
#include "binding-mruby/input.h"

#define MAIN_LAYER_TAG 1000

extern pthread_mutex_t s_input_codelist_mutex;

#ifdef WIN32
static void key_handler( UINT message,WPARAM wParam, LPARAM lParam )
{
	Input::ButtonListStruct info={Input::None,0};
	switch (wParam)
	{
	case VK_UP:
		info.code = Input::Up;
		break;
	case VK_DOWN:
		info.code = Input::Down;
		break;
	case VK_LEFT:
		info.code = Input::Left;
		break;
	case VK_RIGHT:
		info.code = Input::Right;
		break;
	case VK_RETURN:
		info.code = Input::C;
		break;
	case VK_ESCAPE:
		info.code = Input::B;
		break;
	}

	switch (message)
	{
	case WM_KEYDOWN:
		info.isDown = 1;
		break;
	case WM_KEYUP:
		info.isDown = 0;
		break;
	}

	pthread_mutex_lock(&s_input_codelist_mutex);
	Input::getInstance()->pushkey(info);
	pthread_mutex_unlock(&s_input_codelist_mutex);
}
#endif

CCScene* SceneMain::scene()
{
	CCScene *scene = CCScene::create();
	SceneMain* layer = SceneMain::create();
	scene->addChild(layer);
	layer->setTag(MAIN_LAYER_TAG);
	return scene;
}


bool SceneMain::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
#ifdef WIN32
	CCEGLView::sharedOpenGLView()->setAccelerometerKeyHook(key_handler);
#endif
	
	ThreadHandlerMananger::getInstance();

	m_engine = new RubyEngine;
	mrb_state* mrb = m_engine->initAll();

	m_engine->initRMXPScript("Data/Scripts.rxdata");
	
	scheduleUpdate();

	return true;
}

extern pthread_mutex_t s_thread_handler_mutex;
void SceneMain::update( float delta )
{
	if (!m_engine->getRunRMXP())
		m_engine->runRMXPScript();
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->update(delta);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

CCLayer* SceneMain::getMainLayer()
{
	CCLayer* mainlayer = dynamic_cast<CCLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(MAIN_LAYER_TAG));
	return mainlayer;
}

