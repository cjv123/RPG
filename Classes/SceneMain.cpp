#include "SceneMain.h"
#include "binding-mruby/RubyEngine.h"
#include "RMSceneCache.h"

CCScene* SceneMain::scene()
{
	CCScene *scene = CCScene::create();
	SceneMain* layer = SceneMain::create();
	scene->addChild(layer);
	return scene;
}


bool SceneMain::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	RMSceneCache* sceneCache = RMSceneCache::getInstance();
	sceneCache->init(getContentSize().width,getContentSize().height);
	addChild(sceneCache);

	RubyEngine* engine = new RubyEngine;
	mrb_state* mrb = engine->initAll();
	
	engine->initRMXPScript("Data/Scripts.rxdata");

	engine->runRMXPScript();

	return true;
}

void SceneMain::update( float delta )
{

}

