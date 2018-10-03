#include "BoidLogicHandler.h"

//Helper functions
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

glm::vec3 BoidLogicHandler::LimitSpeed(glm::vec3 oldVelocity, glm::vec3 newVelocity, float deltaTime) {
	glm::vec3 limitedVelocity = newVelocity;
	float newSpeed = glm::length(newVelocity);
	float oldSpeed = glm::length(oldVelocity);

	if (newSpeed > MAX_SPEED || newSpeed < MIN_SPEED) {
		limitedVelocity = oldVelocity;
	}
	else {
		if (newSpeed > oldSpeed) {
			limitedVelocity = glm::normalize(limitedVelocity) * (oldSpeed + (MAX_ACCELERATION * deltaTime));
		}
		else {
			limitedVelocity = glm::normalize(limitedVelocity) * (oldSpeed - (MAX_ACCELERATION * deltaTime));
		}
	}

	return limitedVelocity;
}

glm::vec3 BoidLogicHandler::CalculateNewPos(glm::vec3 oldPosition, glm::vec3 newVelocity, float deltaTime) {
	glm::vec3 newPos = oldPosition + (newVelocity * deltaTime * BOID_SPEED);

	return newPos;
}

glm::vec3 BoidLogicHandler::MoveIfOutOfBounds(glm::vec3 position) {
	glm::vec3 newPosition = position;

	float sideLength = GRID_SIDE_LENGTH;

	float xMax = 0.0f + (sideLength / (float)2);
	float xMin = 0.0f - (sideLength / (float)2);
	float yMax = 0.0f + (sideLength / (float)2);
	float yMin = 0.0f - (sideLength / (float)2);
	float zMax = 0.0f + (sideLength / (float)2);
	float zMin = 0.0f - (sideLength / (float)2);

	//X
	if (position.x > xMax) {
		newPosition.x = xMin;
	}
	if (position.x < xMin) {
		newPosition.x = xMax;
	}
	//Y
	if (position.y > yMax) {
		newPosition.y = yMin;
	}
	if (position.y < yMin) {
		newPosition.y = yMax;
	}
	//Z
	if (position.z > zMax) {
		newPosition.z = zMin;
	}
	if (position.z < zMin) {
		newPosition.z = zMax;
	}

	return newPosition;
}

void BoidLogicHandler::BoidThread(Scene* scene, int startIndex, int endIndex, float deltaTime) {
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	Boid* allBoids = scene->GetAllBoids();
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
		newVelocity = LimitSpeed(previousVelocity, newVelocity, deltaTime);

		//Set new boid velocity and up direction
		allBoids[i].SetVelocityAndUp(newVelocity);

		//Calculate new boid position
		glm::vec3 oldPosition = allBoidsPrevious[i].GetPosition();
		glm::vec3 newPosition = CalculateNewPos(oldPosition, newVelocity, deltaTime);

		//Move if out of bounds
		newPosition = MoveIfOutOfBounds(newPosition);

		//Set boid new position
		allBoids[i].SetPosition(newPosition);
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
	
	delete this->constantsBuffer;
	this->constantsBuffer = nullptr;
	
	delete this->deltaTimeBuffer;
	this->deltaTimeBuffer = nullptr;
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
	scene->SwitchCurrentAndPreviousBoids();
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	Boid* allBoids = scene->GetAllBoids();
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
		newVelocity = LimitSpeed(previousVelocity, newVelocity, deltaTime);

		//Set new boid velocity and up direction
		allBoids[i].SetVelocityAndUp(newVelocity);

		//Calculate new boid position
		glm::vec3 oldPosition = allBoidsPrevious[i].GetPosition();
		glm::vec3 newPosition = CalculateNewPos(oldPosition, newVelocity, deltaTime);

		//Move if out of bounds
		newPosition = MoveIfOutOfBounds(newPosition);

		//Set boid new position
		allBoids[i].SetPosition(newPosition);
	}

	scene->GetBoidBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}

void BoidLogicHandler::MultiThreadUpdate(Scene* scene, float deltaTime) {
	scene->SwitchCurrentAndPreviousBoids();
	const int THREADS = 8;
	std::thread threadPool[THREADS];

	int startIndex = 0;
	int endIndex = 0;

	for (int i = 0; i < THREADS; i++) {
		startIndex = i * (NR_OF_BOIDS / THREADS);
		endIndex = (i * (NR_OF_BOIDS / THREADS)) + (NR_OF_BOIDS / THREADS);
		threadPool[i] = std::thread(BoidThread, scene, startIndex, endIndex, deltaTime);
	}

	for (auto& th : threadPool) {
		th.join();
	}


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
	dxContext->Dispatch(NR_OF_BOIDS/64, 1, 1);

	//Null resources
	ID3D11ShaderResourceView* srvNullArray[] = { nullptr };
	ID3D11UnorderedAccessView* uavNullArray[] = { nullptr };
	dxContext->CSSetShaderResources(0, 1, srvNullArray);
	dxContext->CSSetUnorderedAccessViews(0, 1, uavNullArray, 0);

	//Unset computeshader
		dxContext->CSSetShader(nullptr,
		nullptr,
		0);

	//Switch buffers for next frame
	boidBufferSwitchIndex = 1 - boidBufferSwitchIndex;
}