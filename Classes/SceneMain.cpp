#include "SceneMain.h"
#include "ThreadHandlerManager.h"
#include "binding-mruby/input.h"

#define MAIN_LAYER_TAG 1000

extern pthread_mutex_t s_input_codelist_mutex;

static GamePad* g_gamepad=NULL;

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
	g_gamepad = GamePad::create();
	scene->addChild(g_gamepad,10);

	SceneMain* layer = SceneMain::create();
	scene->addChild(layer);
	CCSize sceneSize = scene->getContentSize();
	layer->setTag(MAIN_LAYER_TAG);
	layer->setContentSize(CCSizeMake(640,480));
	layer->setScale(min(sceneSize.width/layer->getContentSize().width,
		sceneSize.height/layer->getContentSize().height));
	layer->setPosition(ccp(sceneSize.width/2 - layer->getContentSize().width/2,sceneSize.height/2-layer->getContentSize().height/2));

	return scene;
}


bool SceneMain::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();

	m_lastkey = 0;

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

	if(g_gamepad)
	{
		updateGamePad(delta);
		g_gamepad->update(delta);
	}

	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->update(delta);
	pthread_mutex_unlock(&s_thread_handler_mutex);

}

CCLayer* SceneMain::getMainLayer()
{
	CCLayer* mainlayer = dynamic_cast<CCLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(MAIN_LAYER_TAG));
	return mainlayer;
}

extern int g_frame_rate;
void SceneMain::updateGamePad( float delta )
{
	Input::ButtonListStruct info={Input::None,0};
	if (g_gamepad->isPress(GamePad::Button_Up))
	{
		info.code = Input::Up;
		info.isDown =1;
	}
	else if (g_gamepad->isPress(GamePad::Button_Down))
	{
		info.code = Input::Down;
		info.isDown =1;
	}
	else if (g_gamepad->isPress(GamePad::Button_Left))
	{
		info.code = Input::Left;
		info.isDown =1;
	}
	else if (g_gamepad->isPress(GamePad::Button_Right))
	{
		info.code = Input::Right;
		info.isDown =1;
	}
	else if (g_gamepad->isJustPress(GamePad::Button_Up))
	{
		info.code = Input::Up;
	}
	else if (g_gamepad->isJustPress(GamePad::Button_Down))
	{
		info.code = Input::Down;
	}
	else if (g_gamepad->isJustPress(GamePad::Button_Left))
	{
		info.code = Input::Left;
	}
	else if (g_gamepad->isJustPress(GamePad::Button_Right))
	{
		info.code = Input::Right;
	}
	
	if (g_gamepad->isPress(GamePad::Button_Back))
	{
		info.code = Input::B;
		info.isDown =1;
	}
	else if (g_gamepad->isJustPress(GamePad::Button_Back))
	{
		info.code = Input::B;
	}
	else if (g_gamepad->isPress(GamePad::Button_Menu))
	{
		info.code = Input::C;
		info.isDown =1;
	}
	else if (g_gamepad->isJustPress(GamePad::Button_Menu))
	{
		info.code = Input::C;
	}


	if (info.code!=Input::None)
	{
		pthread_mutex_lock(&s_input_codelist_mutex);
		if (!info.isDown || 
			(Input::getInstance()->getKeyStatus(info.code) != Input::Button_State_Down && Input::getInstance()->getKeyStatus(info.code) != Input::Button_State_Just_Down))
		{
			Input::getInstance()->pushkey(info);
		}
		
		pthread_mutex_unlock(&s_input_codelist_mutex);
	}
	
}

string SceneMain::writeablePath;

