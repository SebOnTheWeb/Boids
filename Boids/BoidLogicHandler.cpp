#include "BoidLogicHandler.h"

//Constructors and deconstructors
BoidLogicHandler::BoidLogicHandler(Renderer* rendererPtr) {
	this->rendererPtr = rendererPtr;
}

BoidLogicHandler::~BoidLogicHandler() {

}

//Functions
void BoidLogicHandler::SingleThreadUpdate(Scene* secene, float deltaTime) {

}

void BoidLogicHandler::MultiThreadUpdate(Scene* scene, float deltaTime) {

}

void BoidLogicHandler::GPUUpdate(Scene* scene, float deltaTime) {

}