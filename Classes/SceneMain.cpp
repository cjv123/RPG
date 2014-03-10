#include "SceneMain.h"
#include "ThreadHandlerManager.h"
#include "binding-mruby/input.h"

#define MAIN_LAYER_TAG 1000

#ifdef WIN32
static void key_down(UINT_PTR WPARAM)
{
	switch (WPARAM)
	{
	case VK_UP:
		Input::getInstance()->button_status_map[Input::Up] = Input::Button_State_Just_Down;
		break;
	case VK_DOWN:
		Input::getInstance()->button_status_map[Input::Down] = Input::Button_State_Just_Down;
		break;
	case VK_LEFT:
		Input::getInstance()->button_status_map[Input::Left] = Input::Button_State_Just_Down;
		break;
	case VK_RIGHT:
		Input::getInstance()->button_status_map[Input::Right] = Input::Button_State_Just_Down;
		break;
	}
}

static void key_up(UINT_PTR WPARAM)
{
	switch (WPARAM)
	{
	case VK_UP:
		Input::getInstance()->button_status_map[Input::Up] = Input::Button_State_Up;
		break;
	case VK_DOWN:
		Input::getInstance()->button_status_map[Input::Down] = Input::Button_State_Up;
		break;
	case VK_LEFT:
		Input::getInstance()->button_status_map[Input::Left] = Input::Button_State_Up;
		break;
	case VK_RIGHT:
		Input::getInstance()->button_status_map[Input::Right] = Input::Button_State_Up;
		break;
	}
}

static void key_handler( UINT message,WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_KEYDOWN:
		key_down(wParam);
		break;
	case WM_KEYUP:
		key_up(wParam);
		break;
	}
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

