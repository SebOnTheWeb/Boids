#pragma once

#include "BoidLogicCPUSingle.h"

BoidLogicCPUSingle::BoidLogicCPUSingle(Renderer* rendererPtr, Scene* scenePtr): BoidLogicCPU(rendererPtr, scenePtr) {

}

BoidLogicCPUSingle::~BoidLogicCPUSingle() {

}

void BoidLogicCPUSingle::Update(Scene* scene, float deltaTime) {
	scene->SwitchCurrentAndPreviousBoids();
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	Boid* allBoids = scene->GetAllBoids();
	glm::vec3 newVelocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 previousVelocity = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
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

	scene->GetBoidBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}