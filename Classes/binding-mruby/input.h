#ifndef INPUT_H
#define INPUT_H

#include <map>
using namespace std;

class Input
{
public:
	enum ButtonCode
	{
		None = 0,

		Down = 2, Left = 4, Right = 6, Up = 8,

		A = 11, B = 12, C = 13,
		X = 14, Y = 15, Z = 16,
		L = 17, R = 18,

		Shift = 21, Ctrl = 22, Alt = 23,

		F5 = 25, F6 = 26, F7 = 27, F8 = 28, F9 = 29,

		/* Non-standard extensions */
		MouseLeft = 38, MouseMiddle = 39, MouseRight = 40
	};

	static const int buttonCodeSize;
	enum Button_Status_Type{Button_State_None=0,Button_State_Just_Down,Button_State_Down,Button_State_Up};
	typedef map<ButtonCode,Button_Status_Type> Button_State_Map_Type;
	static Button_State_Map_Type button_status_map;

	static Input* getInstance();
	void update();

	bool isPressed(int button);
	bool isTriggered(int button);
	bool isRepeated(int button);

	int dir4Value();
	int dir8Value();

	/* Non-standard extensions */
	int mouseX();
	int mouseY();
private:
	Input();
	~Input();

};

#endif // INPUT_H
