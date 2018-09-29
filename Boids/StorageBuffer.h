#pragma once

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class Renderer;

class StorageBuffer {
private:
	Renderer* rendererPtr;
	unsigned int nrOfElements;
	unsigned int stride;
	ID3D11Buffer* buffer;
	ID3D11Buffer* stagingBuffer;
	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11UnorderedAccessView* unorderedAccessView;

public:
	//Construtor and deonstructor
	StorageBuffer();
	StorageBuffer(Renderer* rendererPtr, unsigned int nrOfElements, unsigned int stride);
	~StorageBuffer();

	//Getters and setters
	ID3D11Buffer* GetBuffer();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11UnorderedAccessView* GetUnorderedAccessView();

	//Functions
	void SetData(void* data, int sizeOfByte);
	void* GetData();
};