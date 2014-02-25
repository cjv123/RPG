#include "SceneMain.h"
#include "ThreadHandlerManager.h"

#define MAIN_LAYER_TAG 1000

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

