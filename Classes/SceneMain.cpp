#include "SceneMain.h"
#include "binding-mruby/RubyEngine.h"
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

	RubyEngine* engine = new RubyEngine;
	mrb_state* mrb = engine->initAll();

	engine->runScript("c = Color.new(122,156,255,144)\np c.red\np c.green\np c.blue\np c.alpha\n");
	engine->checkException();

// 	engine->initRMXPScript("Data/Scripts.rxdata");
// 	engine->runRMXPScript();
// 
// 	scheduleUpdate();

	return true;
}

extern pthread_mutex_t s_thread_handler_mutex;
void SceneMain::update( float delta )
{
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->update(delta);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

CCLayer* SceneMain::getMainLayer()
{
	CCLayer* mainlayer = dynamic_cast<CCLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(MAIN_LAYER_TAG));
	return mainlayer;
}

