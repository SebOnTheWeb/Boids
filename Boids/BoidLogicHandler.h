#pragma once

#include "Scene.h"
#include "Renderer.h"
#include "Constants.h"
#include <cmath>
#include <thread>

class BoidLogicHandler {
private:
	Renderer* rendererPtr;
	
	ID3D11ComputeShader* computeShader;
	StorageBuffer* constantsBuffer; // Not actually a constant buffer, because reusing of StorageBuffer
	StorageBuffer* deltaTimeBuffer;
	int boidBufferSwitchIndex;

	//Helper functions 
	static glm::vec3 CenterRule(Boid* allBoids, int currentBoidIndex);
	static glm::vec3 AvoidRule(Boid* allBoids, int currentBoidIndex);
	static glm::vec3 VelocityRule(Boid* allBoids, int currentBoidIndex);

	static glm::vec3 LimitSpeed(glm::vec3 oldVelocity, glm::vec3 newVelocity, float deltaTime);
	static glm::vec3 CalculateNewPos(glm::vec3 oldPosition, glm::vec3 newVelocity, float deltaTime);
	static glm::vec3 MoveIfOutOfBounds(glm::vec3 position);

	//Helper functions for multi-threaded CPU
	static void BoidThread(Scene* scene, int startIndex, int endIndex, float deltaTime);

public:
	//Constructors and deconstructor
	BoidLogicHandler(Renderer* rendererPtr);
	~BoidLogicHandler();
	
	//Functions
	void InitCPULogic(Scene* scenePtr);
	void InitGPULogic(Scene* scenePtr);

	void SingleThreadUpdate(Scene* scene, float deltaTime);
	void MultiThreadUpdate(Scene* scene, float deltaTime);
	void GPUUpdate(Scene* scene, float deltaTime);
};