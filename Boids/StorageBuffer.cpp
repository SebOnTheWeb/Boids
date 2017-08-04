#include "StorageBuffer.h"
#include "Renderer.h"

//Constructors and deconstructor
StorageBuffer::StorageBuffer() {
	this->rendererPtr = nullptr;
	this->nrOfElements = 0;
	this->stride = 0;
	this->buffer = nullptr;
	this->stagingBuffer = nullptr;
	this->shaderResourceView = nullptr;
	this->unorderedAccessView = nullptr;
}

StorageBuffer::StorageBuffer(Renderer* rendererPtr, unsigned int nrOfElements, unsigned int stride) {
	this->rendererPtr = rendererPtr;
	this->nrOfElements = nrOfElements;
	this->stride = stride;

	//Create buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = this->stride * this->nrOfElements;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | 
							D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = this->stride;

	HRESULT hr = rendererPtr->GetDxDevice()->CreateBuffer(&bufferDesc, 
		nullptr, 
		&this->buffer);
	assert(hr == S_OK);

	//Create stagingBuffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = this->stride * this->nrOfElements;
	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.BindFlags = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ |
								D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = this->stride;

	hr = rendererPtr->GetDxDevice()->CreateBuffer(&bufferDesc,
		nullptr,
		&this->stagingBuffer);
	assert(hr == S_OK);

	//Create shaderResourceView
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = this->nrOfElements;

	hr = rendererPtr->GetDxDevice()->CreateShaderResourceView(this->buffer,
		&srvDesc,
		&this->shaderResourceView);
	assert(hr == S_OK);

	//Create unorderedAccessView
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = this->nrOfElements;
	uavDesc.Buffer.Flags = 0;

	hr = rendererPtr->GetDxDevice()->CreateUnorderedAccessView(this->buffer,
		&uavDesc,
		&this->unorderedAccessView);
	assert(hr == S_OK);
}

StorageBuffer::~StorageBuffer() {
	if (this->buffer != nullptr) {
		this->buffer->Release();
		this->buffer = nullptr;
	}
	if (this->stagingBuffer != nullptr) {
		this->stagingBuffer->Release();
		this->stagingBuffer = nullptr;
	}
	if (this->shaderResourceView != nullptr) {
		this->shaderResourceView->Release();
		this->shaderResourceView = nullptr;
	}
	if (this->unorderedAccessView != nullptr) {
		this->unorderedAccessView->Release();
		this->unorderedAccessView = nullptr;
	}
}

//Getters and setters
ID3D11Buffer* StorageBuffer::GetBuffer() {
	return this->buffer;
}

ID3D11ShaderResourceView* StorageBuffer::GetShaderResourceView() {
	return this->shaderResourceView;
}

ID3D11UnorderedAccessView* StorageBuffer::GetUnorderedAccessView() {
	return this->unorderedAccessView;
}

//Functions
void StorageBuffer::SetData(void* data, int sizeOfByte) {
	//Move data to stagingBuffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));
	HRESULT hr = rendererPtr->GetDxDeviceContext()->Map(this->stagingBuffer,
		0,
		D3D11_MAP_WRITE,
		0,
		&mappedResource);
	assert(hr == S_OK);

	memcpy(mappedResource.pData, data, sizeOfByte);
	rendererPtr->GetDxDeviceContext()->Unmap(this->stagingBuffer, 0);

	//Copy data to buffer
	rendererPtr->GetDxDeviceContext()->CopyResource(this->buffer, this->stagingBuffer);
}

void* StorageBuffer::GetData() {
	//Move data from buffer to staging buffer
	rendererPtr->GetDxDeviceContext()->CopyResource(this->stagingBuffer, this->buffer);

	//Get data from stagingBuffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));
	HRESULT hr = rendererPtr->GetDxDeviceContext()->Map(this->stagingBuffer,
		0,
		D3D11_MAP_READ,
		0,
		&mappedResource);
	assert(hr == S_OK);

	void* result = mappedResource.pData;
	rendererPtr->GetDxDeviceContext()->Unmap(this->buffer, 0);
	
	return result;
}