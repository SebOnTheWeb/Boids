#include "Renderer.h"

//Helper functions
void Renderer::InitD3D11() {
	//Init
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG; //TODO: Is this right for optimal performance?
	this->dxDevice = nullptr;
	this->dxDeviceContext = nullptr;
	this->swapChain = nullptr;
	this->renderTargetView = nullptr;
	this->backBufferTexture = nullptr;
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D11_VIEWPORT viewPort;

	//Create swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = this->windowWidth;
	swapChainDesc.BufferDesc.Height = this->windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = this->hwnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL,
		createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc,
		&this->swapChain, &this->dxDevice, NULL, &this->dxDeviceContext);

	//Create renderTargetView
	swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBufferTexture));

	this->dxDevice->CreateRenderTargetView(backBufferTexture, nullptr, &renderTargetView);

	//Bind renderTargetView
	this->dxDeviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	//Create viewport
	viewPort.Width = static_cast<float>(this->windowWidth);
	viewPort.Height = static_cast<float>(this->windowHeight);
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	//Bind viewport
	this->dxDeviceContext->RSSetViewports(1, &viewPort);

	//Init shaders
	ID3DBlob* shaderBlob;
	ID3DBlob* errorBlob;

	//Vertex shader
	hr = D3DCompileFromFile(L"Default_VS.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		0,
		0,
		&shaderBlob,
		&errorBlob);

	hr = this->dxDevice->CreateVertexShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&this->vertexShader);

	//Geometry shader
	hr = D3DCompileFromFile(L"Default_GS.hlsl",
		nullptr,
		nullptr,
		"main",
		"gs_5_0",
		0,
		0,
		&shaderBlob,
		&errorBlob);

	hr = this->dxDevice->CreateGeometryShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&this->geometryShader);

	//Pixel shader
	hr = D3DCompileFromFile(L"Default_PS.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		0,
		0,
		&shaderBlob,
		&errorBlob);

	hr = this->dxDevice->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&this->pixelShader);
}


void Renderer::DeInitD3D11() {
	this->backBufferTexture->Release();
	this->renderTargetView->Release();
	this->swapChain->Release();
	this->dxDeviceContext->Release();
	this->dxDevice->Release();
}

//Constructors and deconstructor
Renderer::Renderer(HWND hwnd, HINSTANCE hInstance, unsigned int windowWidth, unsigned int windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->hwnd = hwnd;

	InitD3D11();
}

Renderer::~Renderer() {
	DeInitD3D11();
}

//Getters and setters
ID3D11Device* Renderer::GetDxDevice() {
	return this->dxDevice;
}

ID3D11DeviceContext* Renderer::GetDxDeviceContext() {
	return this->dxDeviceContext;
}

//Functions
void Renderer::Render(const Scene &scene) {
	float clearColor[4] = { 1, 0, 0, 1 };
	dxDeviceContext->ClearRenderTargetView(this->renderTargetView, clearColor);

	// TODO: Render using scene
	this->dxDeviceContext->VSSetShader(this->vertexShader,
		nullptr,
		0);
	this->dxDeviceContext->GSSetShader(this->geometryShader,
		nullptr,
		0);
	this->dxDeviceContext->PSSetShader(this->pixelShader,
		nullptr,
		0);

	this->dxDeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//TODO: Set blendState

	this->dxDeviceContext->OMSetRenderTargets(1,
		&this->renderTargetView,
		nullptr); //TODO: Set depthStencilView

	//Set shader resources

	this->dxDeviceContext->Draw(3, 0);
	
	//Unset shaders
	this->dxDeviceContext->VSSetShader(nullptr, nullptr, 0);
	this->dxDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->dxDeviceContext->PSSetShader(nullptr, nullptr, 0);

	//Unset blendState
}

void Renderer::Present() {
	swapChain->Present(0, 0);
}