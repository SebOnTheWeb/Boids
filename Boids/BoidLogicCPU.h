#pragma once

#include "BoidLogic.h"

class BoidLogicCPU : public BoidLogic {
protected:
	static void UpdateBoid(Boid* allBoidsPrevious, Boid* allBoids, int currentBoidIndex, float deltaTime);

private:
	static glm::vec3 CalculateBaseRulesVelocity(Boid* allBoids, int currentBoidIndex);

	static glm::vec3 CenterRule(Boid* allBoids, int currentBoidIndex);
	static glm::vec3 AvoidRule(Boid* allBoids, int currentBoidIndex);
	static glm::vec3 VelocityRule(Boid* allBoids, int currentBoidIndex);

	static glm::vec3 LimitSpeed(glm::vec3 oldVelocity, glm::vec3 newVelocity, float deltaTime);
	static glm::vec3 CalculateNewPos(glm::vec3 oldPosition, glm::vec3 newVelocity, float deltaTime);
	static glm::vec3 MoveIfOutOfBounds(glm::vec3 position);

public:
	BoidLogicCPU(Scene* scenePtr);
	~BoidLogicCPU();

	virtual void Update(Scene* scenePtr, float deltaTime) = 0;
};