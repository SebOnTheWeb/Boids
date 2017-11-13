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
	void moveBoids(Scene* scene, float deltaTime); //TODO: Remove this and do for each boid instead

	static glm::vec3 CenterRule(Boid* allBoids, int currentBoidIndex);
	static glm::vec3 AvoidRule(Boid* allBoids, int currentBoidIndex);
	static glm::vec3 VelocityRule(Boid* allBoids, int currentBoidIndex);

	static glm::vec3 LimitSpeed(glm::vec3 oldVelocity, glm::vec3 newVelocity);

	//Helper functions for multi-threaded CPU
	static void BoidThread(Scene* scene, int startIndex, int endIndex);

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