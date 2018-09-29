#include "Renderer.h"

//Helper functions
void Renderer::InitD3D11() {
	//Init
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED; // CHANGE FLAG
	//UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
	this->dxDevice = nullptr;
	this->dxDeviceContext = nullptr;
	this->swapChain = nullptr;
	this->renderTargetView = nullptr;
	this->backBufferTexture = nullptr;
	this->depthStencilView = nullptr;
	this->depthBufferTexture = nullptr;
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

	//Create depthStencilView
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = this->windowWidth;
	depthTextureDesc.Height = this->windowHeight;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;

	this->dxDevice->CreateTexture2D(&depthTextureDesc, nullptr, &depthBufferTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	this->dxDevice->CreateDepthStencilView(this->depthBufferTexture, &dsvDesc, &this->depthStencilView);

	//Create renderTargetView
	swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&this->backBufferTexture));

	this->dxDevice->CreateRenderTargetView(this->backBufferTexture, nullptr, &this->renderTargetView);

	//Bind renderTargetView
	this->dxDeviceContext->OMSetRenderTargets(1, &this->renderTargetView, this->depthStencilView);

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
		//Default_VS
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

		//Grid_VS
	hr = D3DCompileFromFile(L"Grid_VS.hlsl",
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
		&this->gridVertexShader);

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
	this->depthBufferTexture->Release();
	this->depthStencilView->Release();
	this->backBufferTexture->Release();
	this->renderTargetView->Release();
	this->swapChain->Release();
	this->dxDeviceContext->Release();
	this->dxDevice->Release();
	this->vertexShader->Release();
	this->geometryShader->Release();
	this->pixelShader->Release();
}

//Constructors and deconstructor
Renderer::Renderer(HWND hwnd, unsigned int windowWidth, unsigned int windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->hwnd = hwnd;

	InitD3D11();
	this->metaDataBufferPtr = new StorageBuffer(this, 1, sizeof(glm::mat4));
}

Renderer::~Renderer() {
	DeInitD3D11();
	delete this->metaDataBufferPtr;
	this->metaDataBufferPtr = nullptr;
}

//Getters and setters
ID3D11Device* Renderer::GetDxDevice() {
	return this->dxDevice;
}

ID3D11DeviceContext* Renderer::GetDxDeviceContext() {
	return this->dxDeviceContext;
}

unsigned int Renderer::GetWindowWidth() const {
	return this->windowWidth;
}

unsigned int Renderer::GetWindowHeight() const {
	return this->windowHeight;
}

//Functions
void Renderer::Render(Scene &scene) {
	float clearColor[4] = { 1, 1, 1, 1 };
	dxDeviceContext->ClearRenderTargetView(this->renderTargetView, clearColor);
	dxDeviceContext->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

	glm::mat4 viewProjection = glm::transpose(
		scene.GetCamera()->GetProjectionMatrix() * scene.GetCamera()->GetViewMatrix());
	this->metaDataBufferPtr->SetData(&viewProjection, sizeof(glm::mat4));


	//Set shader resources
	ID3D11ShaderResourceView* VSsrvs[] = { scene.GetBoidBuffer(0)->GetShaderResourceView() };
	this->dxDeviceContext->VSSetShaderResources(0, 1, VSsrvs);
	ID3D11ShaderResourceView* GSsrvs[] = { metaDataBufferPtr->GetShaderResourceView() };
	this->dxDeviceContext->GSSetShaderResources(0, 1, GSsrvs);

	//Set shaders
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
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	this->dxDeviceContext->OMSetRenderTargets(1,
		&this->renderTargetView,
		this->depthStencilView);

	this->dxDeviceContext->Draw(NR_OF_BOIDS, 0);
	
	//Unset VS and GS
	this->dxDeviceContext->VSSetShader(nullptr, nullptr, 0);
	this->dxDeviceContext->GSSetShader(nullptr, nullptr, 0);

	//############# Grid render #############
	
	ID3D11ShaderResourceView* GridVSsrvs[] = { metaDataBufferPtr->GetShaderResourceView(),
												scene.GetGridCube()->GetGridDataBuffer()->GetShaderResourceView() };
	this->dxDeviceContext->VSSetShaderResources(0, 2, GridVSsrvs);

	this->dxDeviceContext->VSSetShader(this->gridVertexShader,
		nullptr,
		0);

	this->dxDeviceContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	this->dxDeviceContext->Draw(scene.GetGridCube()->GetNrOfGridVertices(), 0);

	//Unset shaders
	this->dxDeviceContext->VSSetShader(nullptr, nullptr, 0);
	this->dxDeviceContext->PSSetShader(nullptr, nullptr, 0);
}

void Renderer::Present() {
	swapChain->Present(0, 0);
}