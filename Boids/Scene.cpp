#include "Scene.h"
#include "Renderer.h"

//Constructiors and deconstructor
Scene::Scene(Renderer* rendererPtr) {
	int startPos = - ((NR_OF_BOIDS * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		int x = startPos + (BOID_SEPERATION * i); //TODO: Evenly divide across room
		int y = 0.0f;//BOID_SEPERATION;
		int z = 0.0f;//BOID_SEPERATION;
		this->boids[i] = Boid(glm::vec3(x, y, z));
	}
	this->camera = Camera(90.0, rendererPtr->GetWindowWidth(), rendererPtr->GetWindowHeight());
	this->rendererPtr = rendererPtr;
	this->storageBuffers[0] = new StorageBuffer(rendererPtr, NR_OF_BOIDS, sizeof(Boid));
	this->storageBuffers[1] = new StorageBuffer(rendererPtr, NR_OF_BOIDS, sizeof(Boid));
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

Boid* Scene::GetAllBoids() {
	return this->boids;
}

Camera* Scene::GetCamera() {
	return &this->camera;
}

StorageBuffer* Scene::GetStorageBuffer(unsigned int index) const {
	assert(index < 2);
	return this->storageBuffers[index];
}