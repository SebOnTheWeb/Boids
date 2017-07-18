#pragma once

#include <stdexcept>
#include "Boid.h"
#include "Camera.h"
#include "StorageBuffer.h"

class Renderer;

const unsigned int NR_OF_BOIDS = 1000;

class Scene {
private:
	Boid boids [NR_OF_BOIDS];
	Camera camera;
	StorageBuffer* storageBuffers [2];

	Renderer* rendererPtr;

public:
	//Constructors and deconstructor
	Scene(Renderer* rendererPtr);
	~Scene();

	//Getters and setters
	Boid GetBoid(unsigned int index) const;
	Camera GetCamera() const;
	StorageBuffer* GetStorageBuffer(unsigned int index) const;
};