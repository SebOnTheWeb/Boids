#include "BoidLogicGPU.h"

BoidLogicGPU::BoidLogicGPU(Renderer* rendererPtr, Scene* scenePtr) : BoidLogic() {
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

	scenePtr->GetBoidBuffer(0)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
	scenePtr->GetBoidBuffer(1)->SetData(scenePtr->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);

	constantsBuffer->SetData(&constants, sizeof(Constants));
}

BoidLogicGPU::~BoidLogicGPU() {
	this->computeShader->Release();

	delete this->constantsBuffer;
	this->constantsBuffer = nullptr;

	delete this->deltaTimeBuffer;
	this->deltaTimeBuffer = nullptr;
}

void BoidLogicGPU::Update(Scene* scene, float deltaTime) {
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
	dxContext->Dispatch(NR_OF_BOIDS / 64, 1, 1);

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