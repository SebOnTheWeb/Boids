#pragma once

#include "Scene.h"
#include "Renderer.h"

const float			MIN_SEPERATION_DISTANCE = 1.0f;
const unsigned int	COHESION_THRESHHOLD = 2;
const float			MAX_SPEED = 1.0f;
const float			MIN_SPEED = 0.001f;
const unsigned int	MAX_ROTATION = 20;

class BoidLogicHandler {
private:
	Renderer* rendererPtr;
	
	ID3D11ComputeShader* computeShader;

	//Helper functions //TODO: Helper only for CPU?
	void moveBoids(Scene* scene, float deltaTime);

public:
	//Constructors and deconstructor
	BoidLogicHandler(Renderer* rendererPtr);
	~BoidLogicHandler();
	
	//Functions
	void InitCPULogic(Scene* scenePtr);
	void InitGPULogic();

	void SingleThreadUpdate(Scene* scene, float deltaTime);
	void MultiThreadUpdate(Scene* scene, float deltaTime);
	void GPUUpdate(Scene* scene, float deltaTime);
};