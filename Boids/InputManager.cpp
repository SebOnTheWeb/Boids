#include "InputManager.h"

//Constructor and deconstructor
InputManager::InputManager() {

}

InputManager::InputManager(int windowWidth, int windowHeight) {
	this->previousMousePos = glm::vec2(0.0f, 0.0f);
	this->currentMousePos = glm::vec2(0.0f, 0.0f);

	this->spaceIsPressed = false;

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

InputManager::~InputManager() {

}

//Functions
void InputManager::Update() {
	this->previousMousePos = this->currentMousePos;
	POINT point;
	GetCursorPos(&point);
	this->currentMousePos.x = point.x;
	this->currentMousePos.y = point.y;
}

bool InputManager::KeyPressed(int virtualKey) {
	return (bool)GetAsyncKeyState(virtualKey);
}

bool InputManager::SpaceDown() {
	bool result = false;

	if ((!this->KeyPressed(VK_SPACE)) && spaceIsPressed) {
		spaceIsPressed = false;
	}
	else if (this->KeyPressed(VK_SPACE) && !spaceIsPressed) {
		spaceIsPressed = true;
		result = true;
	}

	return result;
}

glm::vec2 InputManager::MouseDeltaMovement() {
	glm::vec2 result = glm::vec2(0.0f, 0.0f);
	result = this->currentMousePos - this->previousMousePos;

	return result;
}