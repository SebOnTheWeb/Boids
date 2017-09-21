#pragma once

#include <stdexcept>
#include <time.h>
#include "Boid.h"
#include "Camera.h"
#include "StorageBuffer.h"
#include "GridCube.h"

class Renderer;

const unsigned int NR_OF_BOIDS = 100; //Must be dividable by 10
const float BOID_SEPERATION = 2.0f; //Init position seperation


class Scene {
private:
	Boid* boidsPrevious = new Boid[NR_OF_BOIDS];
	Boid* boids = new Boid[NR_OF_BOIDS];
	GridCube* gridCube;
	Camera camera;
	StorageBuffer* storageBuffers[2];

	Renderer* rendererPtr;

public:
	//Constructors and deconstructor
	Scene(Renderer* rendererPtr);
	~Scene();

	//Getters and setters
	Boid GetBoid(unsigned int index) const;
	Boid* GetAllBoids();
	Boid* GetAllBoidsPrevious();
	GridCube* GetGridCube();
	Camera* GetCamera();
	StorageBuffer* GetStorageBuffer(unsigned int index) const;

	void SetBoidVelocity(unsigned int index, glm::vec3 newVelocity);
};