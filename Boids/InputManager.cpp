#include "InputManager.h"

//Constructor and deconstructor
InputManager::InputManager() {
	this->input = nullptr;
	this->keyboard = nullptr;
	this->mouse = nullptr;
}

InputManager::InputManager(HINSTANCE* hInstance, HWND* hwnd, int windowWidth, int windowHeight) {
	this->input = nullptr;
	this->keyboard = nullptr;
	this->mouse = nullptr;

	HRESULT hr = DirectInput8Create(*hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&this->input,
		NULL);
	assert(hr == S_OK);

	//Init keyboard 
	hr = input->CreateDevice(GUID_SysKeyboard,
		&this->keyboard,
		NULL);
	assert(hr == S_OK);

	hr = this->keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(hr == S_OK);

	hr = this->keyboard->SetCooperativeLevel(*hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(hr == S_OK);

	hr = this->keyboard->Acquire();
	assert(hr == S_OK);

	//Init mouse
	hr = input->CreateDevice(GUID_SysMouse,
		&this->mouse,
		NULL);
	assert(hr == S_OK);

	hr = this->mouse->SetDataFormat(&c_dfDIMouse);
	assert(hr == S_OK);

	hr = this->mouse->SetCooperativeLevel(*hwnd,
		DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	assert(hr == S_OK);

	hr = this->mouse->Acquire();
	assert(hr == S_OK);

	this->mousePosX = 0;
	this->mousePosY = 0;

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

InputManager::~InputManager() {
	keyboard->Unacquire();
	keyboard->Release();
	keyboard = nullptr;
	mouse->Unacquire();
	mouse->Release();
	mouse = nullptr;
	input->Release();
	input = nullptr;
}

//Functions
void InputManager::Update() {
	HRESULT hr;

	//Update keyboard state
	hr = this->keyboard->GetDeviceState(sizeof(this->keyboardCurrentState),
		(LPVOID)&this->keyboardCurrentState);

	//Try to regain state if lost
	if (FAILED(hr)) {
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			this->keyboard->Acquire();
		}
		else {
			assert(hr == S_OK); // Kind of ugly but hey :)
		}
	}

	//Update mouse state
	this->mouse->GetDeviceState(sizeof(DIMOUSESTATE),
		(LPVOID)&this->mouseState);

	//Try to regain state if lost
	if (FAILED(hr)) {
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			this->mouse->Acquire();
		}
		else {
			assert(hr == S_OK); // Kind of ugly but hey :)
		}
	}

	this->mousePosX += this->mouseState.lX;
	this->mousePosY += this->mouseState.lY;
	if (this->mousePosX < 0) { this->mousePosX = 0; }
	if (this->mousePosY < 0) { this->mousePosY = 0; }
	if (this->mousePosX > this->windowWidth) { this->mousePosX = this->windowWidth; }
	if (this->mousePosY < this->windowHeight) { this->mousePosY = this->windowHeight; }
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
	return ((this->mouseState.lX != 0)
		|| (this->mouseState.lY != 0));
}

glm::vec2 InputManager::MouseDeltaMovement() {
	glm::vec2 result = glm::vec2(0.0f, 0.0f);
	result.x = this->mouseState.lX;
	result.y = this->mouseState.lY;
	return result;
}