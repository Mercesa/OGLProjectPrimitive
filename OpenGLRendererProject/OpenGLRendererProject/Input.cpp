#include "Input.h"

using namespace Primitive;


Input::Input()
{
	mousePosition.x = 0;
	mousePosition.y = 0;
	std::cout << "Input class constructor" << std::endl;
	Init();
}


Input::~Input()
{}

// Need to make update function to reset the keys every frae
void Input::Init()
{
	for (int i = 0; i < amountOfKeys; i++)
	{
		keysDown[i] = 0;
		keysUp[i] = 1;
		keys[i] = 0;
	}

	for (int j = 0; j < amountOfMouseButtons; j++)
	{
		mouseButtonsDown[j] = false;
		mouseButtonsUp[j] = false;
	}
}

void Input::Update()
{
	for (int i = 0; i < amountOfKeys; i++)
	{
		keysDown[i] = false;
		keysUp[i] = false;
	}

}

void Input::MouseMove(int _X, int _Y)
{
	mousePosition.x = _X;
	mousePosition.y = _Y;
	//std::cout << mousePosition.x << std::endl;
}

void Input::MouseDown(int _Button)
{
	switch (_Button)
	{
	case 1:
		mouseButtonsDown[LEFT] = 1;
		mouseButtonsUp[LEFT] = 0;
		break;

	case 2:
		mouseButtonsDown[MIDDLE] = 1;
		mouseButtonsUp[MIDDLE] = 0;
		break;

	case 3:
		mouseButtonsDown[RIGHT] = 1;
		mouseButtonsUp[RIGHT] = 0;
		break;
	}
}

// left = 1, middle = 2, right = 3
void Input::MouseUp(int _Button)
{
	switch (_Button)
	{
	case 1:
		mouseButtonsDown[LEFT] = 0;
		mouseButtonsUp[LEFT] = 1;
		break;

	case 2:
		mouseButtonsDown[MIDDLE] = 0;
		mouseButtonsUp[MIDDLE] = 1;
		break;

	case 3:
		mouseButtonsDown[RIGHT] = 0;
		mouseButtonsUp[RIGHT] = 1;
		break;
	}
}

void Input::KeyDown(unsigned int a_Key)
{
	//a = 4
	//s = 22
	// d = 7
	// w = 26
	// space = 44
	switch (a_Key)
	{
	case 97:
		if (!keys[A])
		{
			keysDown[A] = true;
			keysUp[A] = false;
		}

		keys[A] = true;
		break;

	case 115:
		if (!keys[S])
		{
			keysDown[S] = true;
			keysUp[S] = false;
		}

		keys[S] = true;
		break;

	case 100:
		if (!keys[D])
		{
			keysDown[D] = true;
			keysUp[D] = false;
		}
		keys[D] = true;
		break;

	case 119:
		if (!keys[W])
		{
			keysDown[W] = true;
			keysUp[W] = false;
		}
		keys[W] = true;
		break;

	case 1073741906:
		if (!keys[UPARROW])
		{
			keysDown[UPARROW] = true;
			keysUp[UPARROW] = false;
		}
		keys[UPARROW] = true;
		break;

	case 1073741905:
		if (!keys[DOWNARROW])
		{
			keysDown[DOWNARROW] = true;
			keysUp[DOWNARROW] = false;
		}
		keys[DOWNARROW] = true;
		break;

	case 1073741904:
		if (!keys[LEFTARROW])
		{
			keysDown[LEFTARROW] = true;
			keysUp[LEFTARROW] = false;
		}
		keys[LEFTARROW] = true;
		break;
	case 1073741903:
		if (!keys[RIGHTARROW])
		{
			keysDown[RIGHTARROW] = true;
			keysUp[RIGHTARROW] = false;
		}
		keys[RIGHTARROW] = true;
		break;
	}
}

void Input::KeyUp(unsigned int a_Key)
{
	// Save letter and do it at the end
	switch (a_Key)
	{
	case 97:
		keysDown[A] = false;
		keysUp[A] = true;
		keys[A] = false;
		break;

	case 115:
		keysDown[S] = false;
		keysUp[S] = true;
		keys[S] = false;
		break;

	case 100:
		keysDown[D] = false;
		keysUp[D] = true;
		keys[D] = false;
		break;

	case 119:

		keysDown[W] = false;
		keysUp[W] = true;
		keys[W] = false;
		break;

	case 32:
		keysDown[SPACE] = false;
		keysUp[SPACE] = true;
		keys[SPACE] = false;

		break;

	case 1073741906:
		keysDown[UPARROW] = false;
		keysUp[UPARROW] = true;
		keys[UPARROW] = false;
		break;

	case 1073741905:
		keysDown[DOWNARROW] = false;
		keysUp[DOWNARROW] = true;
		keys[DOWNARROW] = false;
		break;

	case 1073741904:
		keysDown[LEFTARROW] = false;
		keysUp[LEFTARROW] = true;
		keys[LEFTARROW] = false;
		break;

	case 1073741903:
		keysDown[RIGHTARROW] = false;
		keysUp[RIGHTARROW] = true;
		keys[RIGHTARROW] = false;
		break;
	}
}

bool Input::GetKeyDown(KEYS key)
{
	return keysDown[key];
}

bool Input::GetKeyUp(KEYS key)
{
	return keysUp[key];
}

bool Input::GetKey(KEYS key)
{
	return keys[key];
}
