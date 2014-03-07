#include "input.h"
#include "exception.h"
#include "util.h"

#include <vector>
#include <string.h>

const int Input::buttonCodeSize = 24;

struct InputPrivate
{

	InputPrivate()
	{
	}

	~InputPrivate()
	{

	}
};


Input::Input()
{
	p = new InputPrivate;
}

void Input::update()
{
	
}

bool Input::isPressed(int button)
{
	return false;
}

bool Input::isTriggered(int button)
{
	return false;
}

bool Input::isRepeated(int button)
{
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

Input::~Input()
{
	delete p;
}
