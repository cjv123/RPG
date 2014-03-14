#include "input.h"
#include "exception.h"
#include "util.h"

#include <vector>
#include <string.h>

const int Input::buttonCodeSize = 24;

void Input::update()
{


	
	
}

bool Input::isPressed(int button)
{
	Button_State_Map_Type::iterator it = m_buttonStateMap.find((ButtonCode)button);
	if (it == m_buttonStateMap.end())
		return false;
	Button_Status_Type status = it->second;

	if (status == Button_State_Down)
	{
		return true;
	}
	return false;
}

bool Input::isTriggered(int button)
{
	Button_State_Map_Type::iterator it = m_buttonStateMap.find((ButtonCode)button);
	if (it == m_buttonStateMap.end())
		return false;
	Button_Status_Type status = it->second;

	if (status == Button_State_Down)
	{
		return true;
	}
	return false;
}

bool Input::isRepeated(int button)
{
	Button_State_Map_Type::iterator it = m_buttonStateMap.find((ButtonCode)button);
	if (it == m_buttonStateMap.end())
		return false;
	Button_Status_Type status = it->second;

	if (status == Button_State_Down)
	{
		return true;
	}
	return false;
}


int Input::mouseX()
{
	return 0;
}

int Input::mouseY()
{
	return 0;
}

Input::Input()
{
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Up,Button_State_None));
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Down,Button_State_None));
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Left,Button_State_None));
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Right,Button_State_None));
}


Input::~Input()
{
	
}

Input* Input::getInstance()
{
	static Input input;
	return &input;
}

void Input::pushkey( ButtonListStruct code )
{
	m_buttonCodeList.push_front(code);
}

void Input::popkey( ButtonListStruct code )
{
	m_buttonCodeList.pop_back();
}
