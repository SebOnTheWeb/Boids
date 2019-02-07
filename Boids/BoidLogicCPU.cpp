#pragma once

#include "BoidLogicCPU.h"

glm::vec3 BoidLogicCPU::CenterRule(Boid* allBoids, int currentBoidIndex) {
	glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		center += allBoids[i].GetPosition();
	}
	center -= allBoids[currentBoidIndex].GetPosition();
	center = center / (float)(NR_OF_BOIDS - 1);

	return center * CENTER_FACTOR;
}

glm::vec3 BoidLogicCPU::AvoidRule(Boid* allBoids, int currentBoidIndex) {
	glm::vec3 avoid = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 currentBoidPos = allBoids[currentBoidIndex].GetPosition();
	glm::vec3 vecToBoid = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		if (i != currentBoidIndex) {
			vecToBoid = allBoids[i].GetPosition() - currentBoidPos;
			if (glm::length(vecToBoid) < MIN_SEPERATION_DISTANCE) {
				avoid -= vecToBoid;
			}
		}

	}

	return avoid * AVOID_FACTOR;
}

glm::vec3 BoidLogicCPU::VelocityRule(Boid* allBoids, int currentBoidIndex) {
	glm::vec3 velocity = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		velocity += allBoids[i].GetVelocity();
	}
	velocity -= allBoids[currentBoidIndex].GetVelocity();
	velocity = velocity / (float)(NR_OF_BOIDS - 1);

	return velocity * MATCH_FACTOR;
}

glm::vec3 BoidLogicCPU::LimitSpeed(glm::vec3 oldVelocity, glm::vec3 newVelocity, float deltaTime) {
	glm::vec3 limitedVelocity = newVelocity;
	float newSpeed = glm::length(newVelocity);
	float oldSpeed = glm::length(oldVelocity);

	if (newSpeed > MAX_SPEED || newSpeed < MIN_SPEED) {
		limitedVelocity = oldVelocity;
	}
	else {
		if (newSpeed > oldSpeed) {
			limitedVelocity = glm::normalize(limitedVelocity) * (oldSpeed + (MAX_ACCELERATION * deltaTime));
		}
		else {
			limitedVelocity = glm::normalize(limitedVelocity) * (oldSpeed - (MAX_ACCELERATION * deltaTime));
		}
	}

	return limitedVelocity;
}

glm::vec3 BoidLogicCPU::CalculateNewPos(glm::vec3 oldPosition, glm::vec3 newVelocity, float deltaTime) {
	glm::vec3 newPos = oldPosition + (newVelocity * deltaTime * BOID_SPEED);

	return newPos;
}

glm::vec3 BoidLogicCPU::MoveIfOutOfBounds(glm::vec3 position) {
	glm::vec3 newPosition = position;

	float sideLength = GRID_SIDE_LENGTH;

	float xMax = 0.0f + (sideLength / (float)2);
	float xMin = 0.0f - (sideLength / (float)2);
	float yMax = 0.0f + (sideLength / (float)2);
	float yMin = 0.0f - (sideLength / (float)2);
	float zMax = 0.0f + (sideLength / (float)2);
	float zMin = 0.0f - (sideLength / (float)2);

	//X
	if (position.x > xMax) {
		newPosition.x = xMin;
	}
	if (position.x < xMin) {
		newPosition.x = xMax;
	}
	//Y
	if (position.y > yMax) {
		newPosition.y = yMin;
	}
	if (position.y < yMin) {
		newPosition.y = yMax;
	}
	//Z
	if (position.z > zMax) {
		newPosition.z = zMin;
	}
	if (position.z < zMin) {
		newPosition.z = zMax;
	}

	return newPosition;
}

BoidLogicCPU::BoidLogicCPU(Scene* scenePtr) : BoidLogic() {
	//This is just to get boid positions into gpu for first frame before logic is started
	scenePtr->GetBoidBuffer(0)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

BoidLogicCPU::~BoidLogicCPU() {

}