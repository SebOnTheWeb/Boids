#pragma once

#include <windows.h>
#include <d3d11.h>
#include <stdexcept>

#include "Scene.h"

#pragma comment(lib, "d3d11.lib")

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

	//Functions
	void Render(const Scene &scene);
	void Present();
};