#include "GamePad.h"

#define GAMEPAD_NOTIF_NAME "NOTIF_GAMEPAD"

bool GamePad::init()
{
	if (!CCLayer::init())
		return false;
	setTouchEnabled(true);

	m_handler = NULL;

	mUILayer =UILayer::create();
	mUIWidget = GUIReader::shareReader()->widgetFromJsonFile("gamepad_ui_1.ExportJson");
	mUILayer->addWidget(mUIWidget);
	addChild(mUILayer);

	UIButton* upbutton = (UIButton*)mUIWidget->getChildByName("pad_button_up");
	upbutton->setTag(Button_Up);
	mButtonStatesMap.insert(pair<Button_Name,Button_Status_Type>(Button_Up,Button_State_None));

	UIButton* downbutton = (UIButton*)mUIWidget->getChildByName("pad_button_down");
	downbutton->setTag(Button_Down);
	mButtonStatesMap.insert(pair<Button_Name,Button_Status_Type>(Button_Down,Button_State_None));

	UIButton* leftbutton = (UIButton*)mUIWidget->getChildByName("pad_button_left");
	leftbutton->setTag(Button_Left);
	mButtonStatesMap.insert(pair<Button_Name,Button_Status_Type>(Button_Left,Button_State_None));

	UIButton* rightbutton = (UIButton*)mUIWidget->getChildByName("pad_button_right");
	rightbutton->setTag(Button_Right);
	mButtonStatesMap.insert(pair<Button_Name,Button_Status_Type>(Button_Right,Button_State_None));

	UIButton* backbutton = (UIButton*)mUIWidget->getChildByName("pad_button_back");
	backbutton->setTag(Button_Back);
	mButtonStatesMap.insert(pair<Button_Name,Button_Status_Type>(Button_Back,Button_State_None));

	UIButton* menubutton = (UIButton*)mUIWidget->getChildByName("pad_button_menu");
	menubutton->setTag(Button_Menu);
	mButtonStatesMap.insert(pair<Button_Name,Button_Status_Type>(Button_Menu,Button_State_None));
	
	for (int i=0;i<mUIWidget->getChildren()->count();i++)
	{
		UIWidget* widget = (UIWidget*)mUIWidget->getChildren()->objectAtIndex(i);
		widget->setOpacity(110);
	}
	

	//scheduleUpdate();

	return true;
}

void GamePad::update( float delta )
{
	Button_State_Map_Type::iterator it = mButtonStatesMap.begin();
	for (;it!=mButtonStatesMap.end();it++)
	{
		if (it->second == Button_State_Up)
			it->second = Button_State_None;
	}

	UIButton* pointInButton[5]={0};//每个点只能触发一个按钮，取距离最近一个按钮
	it = mButtonStatesMap.begin();
	for (;it!=mButtonStatesMap.end();it++)
	{
		UIButton* uiButton = (UIButton*)mUIWidget->getChildByTag(it->first);
		for (int i=0;i<5;i++)
		{
			if ((mPoints[i].status == MOUSEDOWN || mPoints[i].status == MOUSEMOVE) && uiButton->hitTest(mPoints[i].point))
			{
				if (pointInButton[i])
				{
					float olddis = mPoints[i].point.getDistance(pointInButton[i]->getPosition());
					float curdis = mPoints[i].point.getDistance(uiButton->getPosition());
					if (curdis<olddis)
					{
						pointInButton[i]->setFocused(false);
						mButtonStatesMap[(Button_Name)pointInButton[i]->getTag()] = Button_State_Up;
						pointInButton[i] = uiButton;

						if(m_handler)
							m_handler(0,it->first);
					}
				}
				else
				{
					if(it->second!=Button_State_Down)
					{
						uiButton->setFocused(true);
						it->second = Button_State_Down;
						pointInButton[i] = uiButton;
						if(m_handler)
							m_handler(1,it->first);
					}
				}
				break;
			}
			else 
			{
				uiButton->setFocused(false);
				if (it->second == Button_State_Down)
				{
					it->second = Button_State_Up;
					if(m_handler)
						m_handler(0,it->first);
				}
			}
		}
	}

	for (int i=0;i<5;i++)
	{
		if (mPoints[i].status == MOUSEUP)
		{
			mPoints[i].status = MOUSENONE;
			mPoints[i].point = ccp(-1,-1);
			mPoints[i].id = -1;
		}
	}
	
}

bool GamePad::isPress( Button_Name buttonName )
{
	Button_State_Map_Type::iterator it = mButtonStatesMap.find(buttonName);
	if (it != mButtonStatesMap.end())
	{
		if (it->second == Button_State_Down)
		{
			return true;
		}
	}

	return false;
}

bool GamePad::isJustPress( Button_Name buttonName )
{
	Button_State_Map_Type::iterator it = mButtonStatesMap.find(buttonName);
	if (it != mButtonStatesMap.end())
	{
		if (it->second == Button_State_Up)
		{
			return true;
		}
	}

	return false;
}

void GamePad::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	for (CCSetIterator it = pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch* pTouch = (CCTouch*)(*it);
		CCPoint touchPoint = pTouch->getLocation();
		for (int i=0;i<5;i++)
		{
			if (mPoints[i].status == MOUSENONE)
			{
				mPoints[i].point = touchPoint;
				mPoints[i].id = pTouch->getID();
				mPoints[i].status = MOUSEDOWN;
				break;
			}
		}
	}
}

void GamePad::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	for (CCSetIterator it = pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch* pTouch = (CCTouch*)(*it);
		CCPoint touchPoint = pTouch->getLocation();
		for (int i=0;i<5;i++)
		{
			if (mPoints[i].id == pTouch->getID())
			{
				mPoints[i].point = touchPoint;
				mPoints[i].status = MOUSEMOVE;
				break;
			}
		}
	}
}

void GamePad::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	for (CCSetIterator it = pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch* pTouch = (CCTouch*)(*it);
		CCPoint touchPoint = pTouch->getLocation();
		for (int i=0;i<5;i++)
		{
			if (mPoints[i].id == pTouch->getID())
			{
				mPoints[i].point = touchPoint;
				mPoints[i].status = MOUSEUP;
				break;
			}
		}
	}
}

void GamePad::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	ccTouchesEnded(pTouches,pEvent);
}

void GamePad::registerWithTouchDispatcher( void )
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,getTouchPriority());
}

void GamePad::setHandler( GAME_PAD_HANDLER funp )
{
	m_handler = funp;
}

