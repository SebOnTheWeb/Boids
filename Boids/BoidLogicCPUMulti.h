#pragma once

#include "BoidLogicCPU.h"
#include <thread>

class BoidLogicCPUMulti : public BoidLogicCPU {
private:
	static void BoidThread(Scene* scene, int startIndex, int endIndex, float deltaTime);

public:
	BoidLogicCPUMulti(Scene* scenePtr);
	~BoidLogicCPUMulti();

	void Update(Scene* scene, float deltaTime);
};