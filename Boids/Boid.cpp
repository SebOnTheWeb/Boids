#pragma once

#include "Boid.h"

//Constructors and deonstructor
Boid::Boid() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->velocity = glm::vec3(0.0, 0.0, 0.0);
}

Boid::Boid(const glm::vec3 &position) {
	this->position = position;
	this->velocity = glm::vec3(0.0, 0.0, 1.0);
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

void Boid::SetVelocity(const glm::vec3 &velocity) {
	this->velocity = velocity;
}

//Functions
