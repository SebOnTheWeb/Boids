#pragma once

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#include <dinput.h>
#include "glm\glm.hpp"

class InputManager {
private:
	IDirectInputDevice8* keyboard;
	BYTE keyboardCurrentState[256];
	
	IDirectInputDevice8* mouse;
	DIMOUSESTATE mouseLastState;
	DIMOUSESTATE mouseCurrentState;
	
	LPDIRECTINPUT8 input;

public:
	//Constructors and deconstructor
	InputManager();
	InputManager(HINSTANCE* hInstance, HWND* hwnd);
	~InputManager();

	//Functions
	void Update();
	bool KeyPressed(char key);
	bool MouseMoved();
	glm::vec2 MouseDeltaMovement();
};