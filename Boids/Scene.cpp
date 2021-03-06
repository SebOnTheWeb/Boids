#include "Scene.h"
#include "Renderer.h"

//Constructiors and deconstructor
Scene::Scene(Renderer* rendererPtr) {
	float x, y, z;
	float startPos;
	int index = 0;

	if (NR_OF_BOIDS == 64) {
		//Plain formation
		startPos = -(((NR_OF_BOIDS / 8) * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;
		for (int i = 0; i < NR_OF_BOIDS / 8; i++) {
			for (int j = 0; j < NR_OF_BOIDS / 8; j++) {
				x = startPos + (BOID_SEPERATION * (float)i);
				y = 0.0f;
				z = startPos + (BOID_SEPERATION * (float)j);

				this->boids[index] = Boid(glm::vec3(x, y, z));
				index++;
			}
		}
	}
	else if (NR_OF_BOIDS == 512) {
		//Cube formation
		startPos = -(((NR_OF_BOIDS / 64) * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;
		for (int i = 0; i < NR_OF_BOIDS / 64; i++) {
			for (int j = 0; j < NR_OF_BOIDS / 64; j++) {
				for (int k = 0; k < NR_OF_BOIDS / 64; k++) {
					x = startPos + (BOID_SEPERATION * (float)i);
					y = startPos + (BOID_SEPERATION * (float)k);
					z = startPos + (BOID_SEPERATION * (float)j);

					this->boids[index] = Boid(glm::vec3(x, y, z));
					index++;
				}
			}
		}
	}
	else if (NR_OF_BOIDS == 4096) {
		//Cube formation
		startPos = -(((NR_OF_BOIDS / 256) * BOID_SEPERATION) / 2) + BOID_SEPERATION / 2;
		for (int i = 0; i < NR_OF_BOIDS / 256; i++) {
			for (int j = 0; j < NR_OF_BOIDS / 256; j++) {
				for (int k = 0; k < NR_OF_BOIDS / 256; k++) {
					x = startPos + (BOID_SEPERATION * (float)i);
					y = startPos + (BOID_SEPERATION * (float)k);
					z = startPos + (BOID_SEPERATION * (float)j);

					this->boids[index] = Boid(glm::vec3(x, y, z));
					index++;
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
	
	//Randomize initial velocities (between -0.05 and 0.05 in all directions)
	srand(time(0));
	for (int i = 0; i < NR_OF_BOIDS; i++) {
		x = ((float)rand() / RAND_MAX) -0.5;
		y = ((float)rand() / RAND_MAX) - 0.5;
		z = ((float)rand() / RAND_MAX) - 0.5;
		this->boids[i].SetVelocityAndUp(x, y, z);
	}

	this->gridCube = new GridCube(rendererPtr, GRID_SIDE_LENGTH, 40, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -(float)(GRID_SIDE_LENGTH / 2));
	this->camera = Camera(90.0, rendererPtr->GetWindowWidth(), rendererPtr->GetWindowHeight(), cameraPos);
	this->rendererPtr = rendererPtr;
	this->boidBuffers[0] = new StorageBuffer(rendererPtr, NR_OF_BOIDS, sizeof(Boid));
	this->boidBuffers[1] = new StorageBuffer(rendererPtr, NR_OF_BOIDS, sizeof(Boid));
}

Scene::~Scene() {
	delete boidBuffers[0];
	boidBuffers[0] = nullptr;
	delete boidBuffers[1];
	boidBuffers[1] = nullptr;
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

Boid* Scene::GetAllBoidsPrevious() {
	return this->boidsPrevious;
}

GridCube* Scene::GetGridCube() {
	return this->gridCube;
}

Camera* Scene::GetCamera() {
	return &this->camera;
}

StorageBuffer* Scene::GetBoidBuffer(unsigned int index) const {
	assert(index < 2);
	return this->boidBuffers[index];
}

//Functions
void Scene::SwitchCurrentAndPreviousBoids() {
	Boid* temp = this->boidsPrevious;
	this->boidsPrevious = this->boids;
	this->boids = temp;
}