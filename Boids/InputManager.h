#pragma once
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#include <dinput.h>
#include "glm\glm.hpp"

class InputManager {
private:
	IDirectInputDevice8* keyboard;
	BYTE keyboardCurrentState[256];
	
	IDirectInputDevice8* mouse;
	DIMOUSESTATE mouseState;
	int mousePosX;
	int mousePosY;
	
	LPDIRECTINPUT8 input;

	int windowWidth;
	int windowHeight;

public:
	//Constructors and deconstructor
	InputManager();
	InputManager(HINSTANCE* hInstance, HWND* hwnd, int windowWidth, int windowHeight);
	~InputManager();

	//Functions
	void Update();
	bool KeyPressed(char key);
	bool MouseMoved();
	glm::vec2 MouseDeltaMovement();
};