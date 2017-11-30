#pragma once

#include "Boid.h"

//Constructors and deonstructor
Boid::Boid() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->velocity = glm::vec3(0.0, 0.0, 1.0);
	this->up = glm::vec3(0.0, 1.0, 0.0);
}

Boid::Boid(const glm::vec3 &position) {
	this->position = position;
	this->velocity = glm::vec3(0.0, 0.0, 1);
	this->up = glm::vec3(0.0, 1.0, 0.0);
}

Boid::~Boid() {

}

//Getters and setters
glm::vec3 Boid::GetPosition() const {
	return this->position;
}

glm::vec3 Boid::GetVelocity() const {
	return this->velocity;
}

void Boid::SetPosition(const glm::vec3 &position) {
	this->position = position;
}

void Boid::SetPosition(float x, float y, float z) {
	this->SetPosition(glm::vec3(x, y, z));
}

void Boid::SetVelocityAndUp(const glm::vec3 &velocity) {
	glm::vec3 forward = glm::normalize(velocity);
	glm::vec3 newRight = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), forward));
	glm::vec3 newUp = glm::cross(forward, newRight);
	this->up = newUp;

	this->velocity = velocity;
}

void Boid::SetVelocityAndUp(float x, float y, float z) {
	this->SetVelocityAndUp(glm::vec3(x, y, z));
}
