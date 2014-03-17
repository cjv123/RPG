#include "input.h"
#include "exception.h"
#include "util.h"

#include <vector>
#include <string.h>
#include <pthread.h>

const int Input::buttonCodeSize = 24;

pthread_mutex_t s_input_codelist_mutex;

void Input::update()
{
	Button_State_Map_Type::iterator mapit = m_buttonStateMap.begin();
	for (;mapit!=m_buttonStateMap.end();mapit++)
	{
		if (mapit->second == Button_State_Up)
		{
			mapit->second = Button_State_None;
		}
		else if (mapit->second == Button_State_Just_Down)
		{
			mapit->second = Button_State_Down;
		}
	}

	pthread_mutex_lock(&s_input_codelist_mutex);
	list<ButtonListStruct>::iterator it = m_buttonCodeList.begin();
	for (;it!=m_buttonCodeList.end();it++)
	{
		if (it->isDown)
		{
			m_buttonStateMap[it->code] = Button_State_Just_Down;
		}
		else
		{
			m_buttonStateMap[it->code] = Button_State_Up;
		}
	}
	m_buttonCodeList.clear();
	pthread_mutex_unlock(&s_input_codelist_mutex);
}

bool Input::isPressed(int button)
{
	Button_State_Map_Type::iterator it = m_buttonStateMap.find((ButtonCode)button);
	if (it == m_buttonStateMap.end())
		return false;
	Button_Status_Type status = it->second;

	if (status == Button_State_Down || status == Button_State_Just_Down)
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

	if (status == Button_State_Just_Down)
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

	if (status == Button_State_Just_Down)
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
	pthread_mutex_init(&s_input_codelist_mutex, NULL);

	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Up,Button_State_None));
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Down,Button_State_None));
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Left,Button_State_None));
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(Right,Button_State_None));
	m_buttonStateMap.insert(Button_State_Map_Type::value_type(C,Button_State_None));
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
