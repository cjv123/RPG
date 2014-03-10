#include "input.h"
#include "exception.h"
#include "util.h"

#include <vector>
#include <string.h>

const int Input::buttonCodeSize = 24;

Input::Button_State_Map_Type Input::button_status_map;


void Input::update()
{
	Button_State_Map_Type::iterator it = button_status_map.begin();
	for (;it!=button_status_map.end();it++)
	{
		if (it->second == Button_State_Up)
		{
			it->second = Button_State_None;
		}
		if (it->second == Button_State_Just_Down)
		{
			it->second == Button_State_Down;
		}
	}
}

bool Input::isPressed(int button)
{
	Button_State_Map_Type::iterator it = button_status_map.find((ButtonCode)button);
	if (it == button_status_map.end())
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
	Button_State_Map_Type::iterator it = button_status_map.find((ButtonCode)button);
	if (it == button_status_map.end())
		return false;
	Button_Status_Type status = it->second;

	if (status == Button_State_Just_Down)
	{
		return true;
	}
	return false;
}

bool Input::isRepeated(int button)
{
	Button_State_Map_Type::iterator it = button_status_map.find((ButtonCode)button);
	if (it == button_status_map.end())
		return false;
	Button_Status_Type status = it->second;

	if (status == Button_State_Up)
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
	button_status_map.insert(Button_State_Map_Type::value_type(Up,Button_State_None));
	button_status_map.insert(Button_State_Map_Type::value_type(Down,Button_State_None));
	button_status_map.insert(Button_State_Map_Type::value_type(Left,Button_State_None));
	button_status_map.insert(Button_State_Map_Type::value_type(Right,Button_State_None));
}


Input::~Input()
{
	
}

Input* Input::getInstance()
{
	static Input input;
	return &input;
}
