#ifndef _SCENEMAIN_H
#define _SCENEMAIN_H

#include <cocos2d.h>
using namespace cocos2d;

class SceneMain : public CCLayer
{
public:
	CREATE_FUNC(SceneMain);
	static CCScene* scene();
	static CCLayer* getMainLayer();
	virtual bool init();
protected:
	virtual void update(float delta);
};

#endif
