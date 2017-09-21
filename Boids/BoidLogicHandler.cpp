#include "BoidLogicHandler.h"

//Helper functions
void BoidLogicHandler::moveBoids(Scene* scene, float deltaTime) {
	Boid* boids = scene->GetAllBoids();
	
	for (int i = 0; i < NR_OF_BOIDS; i++) {
		glm::vec3 newPos = boids[i].GetPosition() + (boids[i].GetVelocity() * deltaTime * BOID_SPEED);

		newPos = scene->GetGridCube()->MoveIfOutOfBounds(newPos);

		boids[i].SetPosition(newPos);
	}
}

glm::vec3 BoidLogicHandler::CenterRule(Boid* allBoids, int currentBoidIndex) {
	glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		center += allBoids[i].GetPosition();
	}
	center -= allBoids[currentBoidIndex].GetPosition();
	center = center / (float)(NR_OF_BOIDS - 1);

	return center * CENTER_FACTOR;
}

glm::vec3 BoidLogicHandler::AvoidRule(Boid* allBoids, int currentBoidIndex) {
	glm::vec3 avoid = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 currentBoidPos = allBoids[currentBoidIndex].GetPosition();
	glm::vec3 vecToBoid = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		if (i != currentBoidIndex) {
			vecToBoid = allBoids[i].GetPosition() - currentBoidPos;
			if (glm::length(vecToBoid) < MIN_SEPERATION_DISTANCE) {
				avoid -= vecToBoid;
			}
		}
		
	}

	return avoid;
}

glm::vec3 BoidLogicHandler::VelocityRule(Boid* allBoids, int currentBoidIndex) {
	glm::vec3 velocity = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		velocity += allBoids[i].GetVelocity();
	}
	velocity -= allBoids[currentBoidIndex].GetVelocity();
	velocity = velocity / (float)(NR_OF_BOIDS - 1);

	return velocity * MATCH_FACTOR;
}

glm::vec3 BoidLogicHandler::LimitSpeed(glm::vec3 oldVelocity, glm::vec3 newVelocity) {
	glm::vec3 limitedVelocity = newVelocity;
	float newSpeed = glm::length(newVelocity);
	float oldSpeed = glm::length(oldVelocity);

	if (newSpeed > MAX_SPEED || newSpeed < MIN_SPEED) {
		limitedVelocity = oldVelocity;
	}
	else {
		if (newSpeed > oldSpeed) {
			limitedVelocity = glm::normalize(limitedVelocity) * (oldSpeed + MAX_ACCELERATION);
		}
		else {
			limitedVelocity = glm::normalize(limitedVelocity) * (oldSpeed + -MAX_ACCELERATION);
		}
	}

	return limitedVelocity;
}

//Constructors and deconstructors
BoidLogicHandler::BoidLogicHandler(Renderer* rendererPtr) {
	this->rendererPtr = rendererPtr;

	ID3DBlob* shaderBlob;
	ID3DBlob* errorBlob;

	//Init compute shader
	HRESULT hr = D3DCompileFromFile(L"BoidUpdate_CS.hlsl",
		nullptr,
		nullptr,
		"main",
		"cs_5_0",
		0,
		0,
		&shaderBlob,
		&errorBlob);

	hr = rendererPtr->GetDxDevice()->CreateComputeShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&this->computeShader);
}

BoidLogicHandler::~BoidLogicHandler() {
	this->computeShader->Release();
}

//Functions
void BoidLogicHandler::InitCPULogic(Scene* scenePtr) {
	//This is just to get boid positions into gpu for first frame before logic is started
	scenePtr->GetStorageBuffer(0)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

void BoidLogicHandler::InitGPULogic() {

}

void BoidLogicHandler::SingleThreadUpdate(Scene* scene, float deltaTime) {
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	glm::vec3 newVelocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 previousVelocity = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		previousVelocity = allBoidsPrevious[i].GetVelocity();
		newVelocity = previousVelocity;

		//1. Fly towards center
		glm::vec3 centerRuleVec = CenterRule(allBoidsPrevious, i);

		//2. Avoid boids
		glm::vec3 avoidRuleVec = AvoidRule(allBoidsPrevious, i);

		//3. Match velocity/direction with all boids
		glm::vec3 velocityRuleVec = VelocityRule(allBoidsPrevious, i);

		//Add all rules
		newVelocity += centerRuleVec + avoidRuleVec + velocityRuleVec;

		//Limit speed
		newVelocity = LimitSpeed(previousVelocity, newVelocity);

		//Set boid velocity
		scene->SetBoidVelocity(i, newVelocity);
	}

	moveBoids(scene, deltaTime);
	scene->GetStorageBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

void BoidLogicHandler::MultiThreadUpdate(Scene* scene, float deltaTime) {

}

void BoidLogicHandler::GPUUpdate(Scene* scene, float deltaTime) {

}