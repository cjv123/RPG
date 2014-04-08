#include "StateLoading.h"
#include "SceneMain.h"


StateLoading::StateLoading()
{

}

StateLoading::~StateLoading()
{

}

CCScene* StateLoading::scene()
{
	CCScene *scene = CCScene::create();
	StateLoading* layer = StateLoading::create();
	scene->addChild(layer);
	return scene;
}


bool StateLoading::init()
{
	if (!CCLayer::init())
		return false;

	CCLayerColor* bg = CCLayerColor::create(ccc4(255,255,255,255));
	addChild(bg);

	CCSprite* zflog = CCSprite::create("zf.png");
	addChild(zflog);
	zflog->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
	zflog->setScale(2.0f);

	CCLabelTTF* loadLabel = CCLabelTTF::create("Loading...","Arial",32);
	loadLabel->setAnchorPoint(ccp(1,0));
	addChild(loadLabel);
	loadLabel->setPosition(ccp(getContentSize().width,0));
	loadLabel->setColor(ccc3(0,0,0));

	RubyEngine* engine = RubyEngine::getInstance();
	mrb_state* mrb = engine->initAll();

	engine->initRMXPScript("Data/Scripts.rxdata");

	schedule(schedule_selector(StateLoading::onLoadComplete), 2.0f);

	return true;
}

void StateLoading::onLoadComplete( float d )
{
	CCScene* homescene = SceneMain::scene();
	CCDirector::sharedDirector()->replaceScene(homescene);
}


