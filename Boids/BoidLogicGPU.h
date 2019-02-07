#pragma once

#include "BoidLogic.h"

class BoidLogicGPU : public BoidLogic {
private:
	Renderer* rendererPtr;

	ID3D11ComputeShader* computeShader;
	StorageBuffer* constantsBuffer; // Not actually a constant buffer, because reusing of StorageBuffer
	StorageBuffer* deltaTimeBuffer;
	int boidBufferSwitchIndex;

public:
	BoidLogicGPU(Renderer* rendererPtr, Scene* scenePtr);
	~BoidLogicGPU();

	void Update(Scene* scene, float deltaTime);
};