#include "Scene.h"
#include "Renderer.h"

//Constructiors and deconstructor
Scene::Scene(Renderer* rendererPtr) {
	for (int i = 0; i < NR_OF_BOIDS; i++) {
		int x = BOID_SEPERATION; //TODO: Evenly divide across room
		int y = BOID_SEPERATION;
		int z = BOID_SEPERATION;
		this->boids[i] = Boid(glm::vec3(x, y, z));
	}
	this->camera = Camera(90.0);
	this->rendererPtr = rendererPtr;
	this->storageBuffers[0] = new StorageBuffer(rendererPtr); // TODO: Don't use default constructor
	this->storageBuffers[1] = new StorageBuffer(rendererPtr); // TODO: Don't use default constructor
}

Scene::~Scene() {
	delete storageBuffers[0];
	storageBuffers[0] = nullptr;
	delete storageBuffers[1];
	storageBuffers[1] = nullptr;
}

//Getters and setters
Boid Scene::GetBoid(unsigned int index) const {
	assert(index < NR_OF_BOIDS);
	return boids[index];
}

Camera Scene::GetCamera() const {
	return this->camera;
}

StorageBuffer* Scene::GetStorageBuffer(unsigned int index) const {
	assert(index < 2);
	return this->storageBuffers[index];
}