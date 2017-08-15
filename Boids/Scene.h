#pragma once

#include <stdexcept>
#include "Boid.h"
#include "Camera.h"
#include "StorageBuffer.h"

class Renderer;

const unsigned int NR_OF_BOIDS = 10; //Must be dividable by 10
const float BOID_SEPERATION = 2.0f;


class Scene {
private:
	Boid boidsPrevious[NR_OF_BOIDS];
	Boid boids[NR_OF_BOIDS];
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
	Camera* GetCamera();
	StorageBuffer* GetStorageBuffer(unsigned int index) const;
};