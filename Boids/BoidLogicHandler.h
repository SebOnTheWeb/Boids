#pragma once

#include "Scene.h"
#include "Renderer.h"

const float			MIN_SEPERATION_DISTANCE = 1.0f;
const unsigned int	COHESION_THRESHHOLD = 2;
const float			MAX_SPEED = 1.0f;
const unsigned int	MAX_ROTATION = 20;

class BoidLogicHandler {
private:
	Renderer* rendererPtr;
	//TODO: Compute shader

public:
	//Constructors and deconstructor
	BoidLogicHandler(Renderer* rendererPtr);
	~BoidLogicHandler();
	
	void SingleThreadUpdate(Scene* scene, float deltaTime);
	void MultiThreadUpdate(Scene* scene, float deltaTime);
	void GPUUpdate(Scene* scene, float deltaTime);
};