#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#include <cocos2d.h>
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include <map>
using namespace std;

typedef void (*GAME_PAD_HANDLER)(int,int);   

class GamePad : public CCLayerRGBA
{
public:
	enum Button_Name{Button_Up=500,Button_Down,Button_Left,Button_Right,Button_Back,Button_Menu};
	enum Button_Status_Type{Button_State_None=0,Button_State_Down,Button_State_Up};
	enum Mouse_Status{MOUSENONE,MOUSEDOWN,MOUSEUP,MOUSEMOVE};

	CREATE_FUNC(GamePad);
	virtual bool init();
	virtual void update(float delta);
	bool isPress(Button_Name buttonName);
	bool isJustPress(Button_Name buttonName);
	void setHandler(GAME_PAD_HANDLER funp);
private:
	virtual void ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent );

	virtual void ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent );

	virtual void ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent );

	virtual void ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent );

	virtual void registerWithTouchDispatcher( void );

	UILayer* mUILayer;
	UIWidget* mUIWidget;

	struct MousePoint
	{
		CCPoint point;
		Mouse_Status status;
		int id;

		MousePoint()
		{
			point = ccp(-1,-1);
			status = MOUSENONE;
			id = -1;
		}
	};
	typedef map<Button_Name,Button_Status_Type> Button_State_Map_Type;
	Button_State_Map_Type mButtonStatesMap;
	MousePoint mPoints[5];
	GAME_PAD_HANDLER m_handler;
};

#endif
