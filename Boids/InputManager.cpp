#include "InputManager.h"

//Constructor and deconstructor
InputManager::InputManager() {
	this->input = nullptr;
	this->keyboard = nullptr;
	this->mouse = nullptr;
}

InputManager::InputManager(HINSTANCE* hInstance, HWND* hwnd) {
	this->input = nullptr;
	this->keyboard = nullptr;
	this->mouse = nullptr;

	DirectInput8Create(*hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&this->input,
		NULL);

	//Init keyboard 
	input->CreateDevice(GUID_SysKeyboard,
		&this->keyboard,
		NULL);

	this->keyboard->SetDataFormat(&c_dfDIKeyboard);
	this->keyboard->SetCooperativeLevel(*hwnd, DISCL_FOREGROUND);

	//Init mouse
	input->CreateDevice(GUID_SysMouse,
		&this->mouse,
		NULL);

	this->mouse->SetDataFormat(&c_dfDIMouse);
	this->mouse->SetCooperativeLevel(*hwnd,
		DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
}

InputManager::~InputManager() {
	keyboard->Unacquire();
	mouse->Unacquire();
	input->Release();
}

//Functions
void InputManager::Update() {
	//Update keyboard state
	this->keyboard->Acquire();
	this->keyboard->GetDeviceState(sizeof(this->keyboardCurrentState),
		(LPVOID)&this->keyboardCurrentState);

	//Update mouse state
	this->mouseLastState = this->mouseCurrentState;

	this->mouse->Acquire();
	this->mouse->GetDeviceState(sizeof(DIMOUSESTATE),
		&this->mouseCurrentState);
}

bool InputManager::KeyPressed(char key) {
	bool result = false;

	switch(key) {
	case 'A':
		if (this->keyboardCurrentState[DIK_A] & 0x80) {
			result = true;
		}
		break;
	case 'D':
		if (this->keyboardCurrentState[DIK_D] & 0x80) {
			result = true;
		}
		break;
	case 'W':
		if (this->keyboardCurrentState[DIK_W] & 0x80) {
			result = true;
		}
		break;
	case 'S':
		if (this->keyboardCurrentState[DIK_S] & 0x80) {
			result = true;
		}
		break;
	case ' ':
		if (this->keyboardCurrentState[DIK_SPACE] & 0x80) {
			result = true;
		}
		break;
	}

	return result;
}

bool InputManager::MouseMoved() {
	return (this->mouseCurrentState.lX != this->mouseLastState.lX
		|| this->mouseCurrentState.lY != this->mouseLastState.lY);
}

glm::vec2 InputManager::MouseDeltaMovement() {
	glm::vec2 result = glm::vec2(0.0f, 0.0f);
	result.x = this->mouseLastState.lX - this->mouseCurrentState.lX;
	result.y = this->mouseLastState.lY - this->mouseCurrentState.lY;
	return result;
}