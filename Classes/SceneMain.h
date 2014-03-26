#ifndef _SCENEMAIN_H
#define _SCENEMAIN_H

#include <cocos2d.h>
using namespace cocos2d;
#include "binding-mruby/RubyEngine.h"
#include "GamePad.h"

class SceneMain : public CCLayer
{
public:
	CREATE_FUNC(SceneMain);
	static CCScene* scene();
	static CCLayer* getMainLayer();
	virtual bool init();
protected:
	RubyEngine* m_engine;
	virtual void update(float delta);
	void updateGamePad(float delta);

	int m_lastkey;
};

#endif
