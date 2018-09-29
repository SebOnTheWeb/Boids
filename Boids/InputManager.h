#pragma once

#include <Windows.h>
#include "glm\glm.hpp"

class InputManager {
private:
	glm::vec2 previousMousePos;
	glm::vec2 currentMousePos;

	//No need for full keymap for this project
	bool spaceIsPressed;

	int windowWidth;
	int windowHeight;

public:
	//Constructors and deconstructor
	InputManager();
	InputManager(int windowWidth, int windowHeight);
	~InputManager();

	//Functions
	void Update();
	bool KeyPressed(int virtualKey);
	bool SpaceDown();
	glm::vec2 MouseDeltaMovement();
};