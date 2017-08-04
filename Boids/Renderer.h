#pragma once

#include <windows.h>
#include <d3d11.h>
#include <stdexcept>
#include <d3dcompiler.h>

#include "Scene.h"
#include "StorageBuffer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class Renderer {
private:
	HWND hwnd;

	ID3D11Device* dxDevice;
	ID3D11DeviceContext* dxDeviceContext;

	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* backBufferTexture;
	ID3D11SamplerState* samplerState;
	ID3D11RasterizerState* rasterizerState;

	StorageBuffer* metaDataBufferPtr;

	ID3D11VertexShader* vertexShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11PixelShader* pixelShader;

	unsigned int windowWidth;
	unsigned int windowHeight;

	//Helper functions
	void InitD3D11();
	void DeInitD3D11();

public:
	//Constructor and deconstructor
	Renderer(HWND hwnd, HINSTANCE hInastance, unsigned int windowWidth = 800, unsigned int windowHeight = 600);
	~Renderer();

	//Getters and setters
	ID3D11Device* GetDxDevice();
	ID3D11DeviceContext* GetDxDeviceContext();
	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;

	//Functions
	void Render(Scene &scene);
	void Present();
};