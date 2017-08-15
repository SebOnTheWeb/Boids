#include "BoidLogicHandler.h"

//Helper functions
void BoidLogicHandler::moveBoids(Scene* scene, float deltaTime) {
	Boid* boids = scene->GetAllBoids();
	
	for (int i = 0; i < NR_OF_BOIDS; i++) {
		glm::vec3 newPos = boids[i].GetPosition() + (boids[i].GetVelocity() * deltaTime);
		boids[i].SetPosition(newPos);
	}
}

//Constructors and deconstructors
BoidLogicHandler::BoidLogicHandler(Renderer* rendererPtr) {
	this->rendererPtr = rendererPtr;
}

BoidLogicHandler::~BoidLogicHandler() {

}

//Functions
void BoidLogicHandler::InitCPULogic(Scene* scenePtr) {
	//This is just to get boid positions into gpu for first frame before logic is started
	scenePtr->GetStorageBuffer(0)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

void BoidLogicHandler::InitGPULogic() {

}

void BoidLogicHandler::SingleThreadUpdate(Scene* scene, float deltaTime) {
	scene->GetStorageBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
	moveBoids(scene, deltaTime);
}

void BoidLogicHandler::MultiThreadUpdate(Scene* scene, float deltaTime) {

}

void BoidLogicHandler::GPUUpdate(Scene* scene, float deltaTime) {

}