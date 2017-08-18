#include "Scene.h"
#include "Renderer.h"

//Constructiors and deconstructor
Scene::Scene(Renderer* rendererPtr) {
	float x, y, z;
	float startPos;
	int index = 0;

	if (NR_OF_BOIDS == 100) {
		//Plain formation
		startPos = -(((NR_OF_BOIDS / 10) * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;
		for (int i = 0; i < NR_OF_BOIDS / 10; i++) {
			for (int j = 0; j < NR_OF_BOIDS / 10; j++) {
				x = startPos + (BOID_SEPERATION * (float)i);
				y = 0.0f;
				z = startPos + (BOID_SEPERATION * (float)j);

				this->boids[index] = Boid(glm::vec3(x, y, z));
				index++;
			}
		}
	}
	else if (NR_OF_BOIDS == 1000) {
		//Cube formation
		startPos = -(((NR_OF_BOIDS / 100) * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;
		for (int i = 0; i < NR_OF_BOIDS / 100; i++) {
			for (int j = 0; j < NR_OF_BOIDS / 100; j++) {
				for (int k = 0; k < NR_OF_BOIDS / 100; k++) {
					x = startPos + (BOID_SEPERATION * (float)i);
					y = startPos + (BOID_SEPERATION * (float)k);
					z = startPos + (BOID_SEPERATION * (float)j);

					this->boids[index] = Boid(glm::vec3(x, y, z));
					index++;
				}
			}
		}
	}
	else if (NR_OF_BOIDS == 10000) {
		//Cube formation, ish
		startPos = -(((22) * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;
		for (int i = 0; i < 22; i++) {
			for (int j = 0; j < 22; j++) {
				for (int k = 0; k < 22; k++) {
					x = startPos + (BOID_SEPERATION * (float)i);
					y = startPos + (BOID_SEPERATION * (float)k);
					z = startPos + (BOID_SEPERATION * (float)j);

					this->boids[index] = Boid(glm::vec3(x, y, z));
					index++;
					if (index == 9999) {
						i = 22;
						j = 22;
						k = 22;
					}
				}
			}
		}
	}
	else {
		//Straight line
		startPos = -((NR_OF_BOIDS * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;
		for (int i = 0; i < NR_OF_BOIDS; i++) {
			x = startPos + (BOID_SEPERATION * (float)i);
			y = 0.0f;
			z = 0.0f;

			this->boids[i] = Boid(glm::vec3(x, y, z));
		}
	}

	this->gridCube = new GridCube(rendererPtr, 20.0f, 20, glm::vec3(0.0f, 0.0f, 0.0f));
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
	delete this->gridCube;
	this->gridCube = nullptr;
}

//Getters and setters
Boid Scene::GetBoid(unsigned int index) const {
	assert(index < NR_OF_BOIDS);
	return boids[index];
}

Boid* Scene::GetAllBoids() {
	return this->boids;
}

GridCube* Scene::GetGridCube() {
	return this->gridCube;
}

Camera* Scene::GetCamera() {
	return &this->camera;
}

StorageBuffer* Scene::GetStorageBuffer(unsigned int index) const {
	assert(index < 2);
	return this->storageBuffers[index];
}