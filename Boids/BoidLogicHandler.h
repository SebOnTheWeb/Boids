#pragma once

#include "Scene.h"
#include "Renderer.h"
#include <cmath>

const float			MIN_SEPERATION_DISTANCE = 2.0f;
const unsigned int	COHESION_THRESHHOLD = 2;
const float			BOID_SPEED = 1.0f;
const float			MAX_SPEED = 8.0f;
const float			MIN_SPEED = 0.5f;
const unsigned int	MAX_ROTATION = 15;
const float			MAX_ACCELERATION = 0.01f;

//Rule factors
const float			CENTER_FACTOR = 0.01f;
const float			MATCH_FACTOR = 0.125;

class BoidLogicHandler {
private:
	Renderer* rendererPtr;
	
	ID3D11ComputeShader* computeShader;

	//Helper functions //TODO: Helper only for CPU?
	void moveBoids(Scene* scene, float deltaTime);

	glm::vec3 CenterRule(Boid* allBoids, int currentBoidIndex);
	glm::vec3 AvoidRule(Boid* allBoids, int currentBoidIndex);
	glm::vec3 VelocityRule(Boid* allBoids, int currentBoidIndex);

	glm::vec3 LimitSpeed(glm::vec3 oldVelocity, glm::vec3 newVelocity);

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