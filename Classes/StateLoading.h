#ifndef _STATELOADING_H
#define _STATELOADING_H

#include <cocos2d.h>
using namespace cocos2d;

class StateLoading : public CCLayer
{
public:
	StateLoading();
	~StateLoading();

	static CCScene* scene();

	virtual bool init();

	CREATE_FUNC(StateLoading);

private:
	void onLoadComplete(float d);
};

#endif
