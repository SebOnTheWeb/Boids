#pragma once

#include <glm\glm.hpp>

const unsigned int MAX_NR_OF_NEAREST_NEIGHBORS = 10;

class Boid {
private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 nearestNeighbors[MAX_NR_OF_NEAREST_NEIGHBORS];
	int nrOfNearestNeighbors;

public:
	//Constructors and deconstructor
	Boid();
	Boid(const glm::vec3 &position);
	~Boid();
	
	//Getters and setters
	glm::vec3 GetPosition() const;
	glm::vec3 GetVelocity() const;
	glm::vec3* GetNearestNeighbors();
	int GetNrOfNearestNeighbors() const;
	void SetPosition(const glm::vec3 &position);
	void SetVelocity(const glm::vec3 &velocity);
	void SetNrOfNearestNeighbors(const int &nrOfNearestNeighbors);

	//Functions
};