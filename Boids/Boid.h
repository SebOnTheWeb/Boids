#pragma once

#include <glm\glm.hpp>

class Boid {
private:
	glm::vec3 position;
	glm::vec3 velocity;

public:
	//Constructors and deconstructor
	Boid();
	Boid(const glm::vec3 &position);
	~Boid();
	
	//Getters and setters
	glm::vec3 GetPosition() const;
	glm::vec3 GetVelocity() const;
	void SetPosition(const glm::vec3 &position);
	void SetVelocity(const glm::vec3 &velocity);

	//Functions
};