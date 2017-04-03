#ifndef _INPUT_H_
#define _INPUT_H_

#include <iostream>


namespace Primitive{

struct XYstruct
{
	float x = 0, y = 0;
};

class Input
{
public:
	Input();
	~Input();

	void Init();
	void Update();

	void MouseUp(int _Button);
	void MouseDown(int _Button);
	void MouseMove(int _X, int _Y);
	void MousePosition(int _X, int _Y) { mouseRelativeMoved.x = static_cast<float>(_X); mouseRelativeMoved.y = static_cast<float>(_Y); }
	void KeyUp(unsigned int a_Key);
	void KeyDown(unsigned int a_Key);

	//void Update();
	const int amountOfKeys = 9;
	int amountOfMouseButtons = 3;

	enum KEYS { W, A, S, D, SPACE, UPARROW, DOWNARROW, LEFTARROW, RIGHTARROW };
	enum MOUSEBUTTONS { LEFT, MIDDLE, RIGHT };

	bool GetKeyDown(KEYS key);
	bool GetKeyUp(KEYS key);
	bool GetKey(KEYS key);

	XYstruct mouseRelativeMoved;

private:
	bool keysDown[9];
	bool keysUp[9];
	bool keys[9];
	bool mouseButtonsDown[3];
	bool mouseButtonsUp[3];
};

}
#endif _INPUT_H_
