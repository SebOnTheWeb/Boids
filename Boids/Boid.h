#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Boid {
private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 up;

public:
	//Constructors and deconstructor
	Boid();
	Boid(const glm::vec3 &position);
	~Boid();
	
	//Getters and setters
	glm::vec3 GetPosition() const;
	glm::vec3 GetVelocity() const;
	void SetPosition(const glm::vec3 &position);
	void SetPosition(float x, float y, float z);
	void SetVelocityAndUp(const glm::vec3 &velocity);
	void SetVelocityAndUp(float x, float y, float z);

};