#pragma once

#include "BoidLogicCPU.h"

class BoidLogicCPUSingle : public BoidLogicCPU {
private:
	

public:
	BoidLogicCPUSingle(Renderer* rendererPtr, Scene* scenePtr);
	~BoidLogicCPUSingle();

	void Update(Scene* scene, float deltaTime);
};