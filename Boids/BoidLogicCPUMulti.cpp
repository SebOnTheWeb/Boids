#pragma once

#include "BoidLogicCPUMulti.h"

void BoidLogicCPUMulti::BoidThread(Scene* scene, int startIndex, int endIndex, float deltaTime) {
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	Boid* allBoids = scene->GetAllBoids();
	glm::vec3 newVelocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 previousVelocity = glm::vec3(0.0, 0.0, 0.0);

	for (int i = startIndex; i < endIndex; i++) {
		previousVelocity = allBoidsPrevious[i].GetVelocity();
		newVelocity = previousVelocity;

		//1. Fly towards center
		glm::vec3 centerRuleVec = CenterRule(allBoidsPrevious, i);

		//2. Avoid boids
		glm::vec3 avoidRuleVec = AvoidRule(allBoidsPrevious, i);

		//3. Match velocity/direction with all boids
		glm::vec3 velocityRuleVec = VelocityRule(allBoidsPrevious, i);

		//Add all rules
		newVelocity += centerRuleVec + avoidRuleVec + velocityRuleVec;

		//Limit speed
		newVelocity = LimitSpeed(previousVelocity, newVelocity, deltaTime);

		//Set new boid velocity and up direction
		allBoids[i].SetVelocityAndUp(newVelocity);

		//Calculate new boid position
		glm::vec3 oldPosition = allBoidsPrevious[i].GetPosition();
		glm::vec3 newPosition = CalculateNewPos(oldPosition, newVelocity, deltaTime);

		//Move if out of bounds
		newPosition = MoveIfOutOfBounds(newPosition);

		//Set boid new position
		allBoids[i].SetPosition(newPosition);
	}
}

BoidLogicCPUMulti::BoidLogicCPUMulti(Scene* scenePtr) : BoidLogicCPU(scenePtr) {

}

BoidLogicCPUMulti::~BoidLogicCPUMulti() {

}

void BoidLogicCPUMulti::Update(Scene* scene, float deltaTime) {
	scene->SwitchCurrentAndPreviousBoids();
	const int THREADS = 8;
	std::thread threadPool[THREADS];

	int startIndex = 0;
	int endIndex = 0;

	for (int i = 0; i < THREADS; i++) {
		startIndex = i * (NR_OF_BOIDS / THREADS);
		endIndex = (i * (NR_OF_BOIDS / THREADS)) + (NR_OF_BOIDS / THREADS);
		threadPool[i] = std::thread(BoidThread, scene, startIndex, endIndex, deltaTime);
	}

	for (auto& th : threadPool) {
		th.join();
	}

	scene->GetBoidBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}