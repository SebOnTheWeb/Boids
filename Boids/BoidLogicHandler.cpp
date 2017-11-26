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

	return avoid * AVOID_FACTOR;
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

void BoidLogicHandler::BoidThread(Scene* scene, int startIndex, int endIndex) {
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	glm::vec3 newVelocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 previousVelocity = glm::vec3(0.0, 0.0, 0.0);

	for (int i = startIndex; i < endIndex; i++) {
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

	this->boidBufferSwitchIndex = 0;

	hr = rendererPtr->GetDxDevice()->CreateComputeShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&this->computeShader);

	this->constantsBuffer = new StorageBuffer(rendererPtr, 1, sizeof(Constants));
	this->deltaTimeBuffer = new StorageBuffer(rendererPtr, 1, sizeof(float));
}

BoidLogicHandler::~BoidLogicHandler() {
	this->computeShader->Release();
}

//Functions
void BoidLogicHandler::InitCPULogic(Scene* scenePtr) {
	//This is just to get boid positions into gpu for first frame before logic is started
	scenePtr->GetBoidBuffer(0)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

void BoidLogicHandler::InitGPULogic(Scene* scenePtr) {
	scenePtr->GetBoidBuffer(0)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
	scenePtr->GetBoidBuffer(1)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);

	constantsBuffer->SetData(&constants, sizeof(Constants));
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

		//Move boid

	}

	moveBoids(scene, deltaTime);
	scene->GetBoidBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

void BoidLogicHandler::MultiThreadUpdate(Scene* scene, float deltaTime) {
	const int THREADS = 8;
	std::thread threadPool[THREADS];

	int startIndex = 0;
	int endIndex = 0;

	for (int i = 0; i < THREADS; i++) {
		startIndex = i * (NR_OF_BOIDS / THREADS);
		endIndex = (i * (NR_OF_BOIDS / THREADS)) + (NR_OF_BOIDS / THREADS);
		threadPool[i] = std::thread(BoidThread, scene, startIndex, endIndex);
	}

	for (auto& th : threadPool) {
		th.join();
	}

	moveBoids(scene, deltaTime);
	scene->GetBoidBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

void BoidLogicHandler::GPUUpdate(Scene* scene, float deltaTime) {
	ID3D11DeviceContext* dxContext = this->rendererPtr->GetDxDeviceContext();

	//Set computeshader
	dxContext->CSSetShader(this->computeShader,
		nullptr,
		0);

	//Set delta time
	this->deltaTimeBuffer->SetData(&deltaTime, sizeof(float));

	//Dispatch shader
	ID3D11ShaderResourceView* srvArray[] = { scene->GetBoidBuffer(boidBufferSwitchIndex)->GetShaderResourceView(),
											this->deltaTimeBuffer->GetShaderResourceView() ,
											this->constantsBuffer->GetShaderResourceView() };
	ID3D11UnorderedAccessView* uavArray[] = { scene->GetBoidBuffer((boidBufferSwitchIndex + 1) % 2)->GetUnorderedAccessView() };
	dxContext->CSSetShaderResources(0, 3, srvArray);
	dxContext->CSSetUnorderedAccessViews(0, 1, uavArray, 0);
	dxContext->Dispatch(NR_OF_BOIDS, 1, 1);

	//Null resources
	ID3D11ShaderResourceView* srvNullArray[] = { nullptr };
	ID3D11UnorderedAccessView* uavNullArray[] = { nullptr };
	dxContext->CSSetShaderResources(0, 1, srvNullArray);
	dxContext->CSSetUnorderedAccessViews(0, 1, uavNullArray, 0);

	//Switch buffers for next frame
	boidBufferSwitchIndex = 1 - boidBufferSwitchIndex;

	//Unset computeshader
		dxContext->CSSetShader(nullptr,
		nullptr,
		0);

	//Move boids TODO: Move to GPU
	moveBoids(scene, deltaTime);
}