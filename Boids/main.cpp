#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include <chrono>

#include <windows.h>
#include <d3d11.h>
#include <fstream>
#include <iostream>
#include <string>

#include "Renderer.h"
#include "Scene.h"
#include "BoidLogicHandler.h"
#include "InputManager.h"
#include "Measurements.h"
#include "Constants.h"

#pragma comment(lib, "d3d11.lib")


HWND CreateShowWindow(int windowWidth, int windowHeight, InputManager* inputManagerPtr);
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	unsigned int windowWidth = 1024;
	unsigned int windowHeight = 800;

	bool isRunning = true;
	bool updateLogic = false;

	//Inited first to get event handler function to window creation
	InputManager* inputManager = new InputManager(windowWidth, windowHeight);

	//Create window
	HWND hwnd = CreateShowWindow(windowWidth, windowHeight, inputManager);

	//Init classes
	Renderer renderer = Renderer(hwnd, windowWidth, windowHeight);
	Scene scene = Scene(&renderer);
	BoidLogicHandler boidLogic = BoidLogicHandler(&renderer);
	Measurements measurements = Measurements(NR_OF_SEC_TO_MEASURE);


	//################## Logic init options #######################
	//boidLogic.InitCPULogic(&scene);
	boidLogic.InitGPULogic(&scene);
	//#############################################################

	//Init timer
	double time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	//Init fps tracking
	int totalSecondTracker = 0;
	double secondTracker = 0.0;
	int fpsCounter = 0;

	//Init update speed timer
	double timeBeforeUpdate = 0.0;
	double timeAfterUpdate = 0.0;
	double totalUpdateTime = 0.0;

	// Run the message loop.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (isRunning)
	{
		//Check if program should exit
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				isRunning = false;
			}
		}

		//Update input
		inputManager->Update();

		//Program logic start/stop
		if (inputManager->SpaceDown()) {
			if (updateLogic) updateLogic = false;
			else updateLogic = true;
		}

		//Timer update
		double previousFrameTime = time;
		time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		float deltaTime = (time - previousFrameTime) / 1000000000;

		if (updateLogic) {
			timeBeforeUpdate = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			//################## Logic update options #####################
			//boidLogic.SingleThreadUpdate(&scene, deltaTime);
			//boidLogic.MultiThreadUpdate(&scene, deltaTime);
			boidLogic.GPUUpdate(&scene, deltaTime);
			//#############################################################
			timeAfterUpdate = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			//FPS and update tracking
			totalUpdateTime += (timeAfterUpdate - timeBeforeUpdate);
			fpsCounter += 1;
			secondTracker += deltaTime;
			if (secondTracker >= 1.0) {
				//Set average time for update this second
				double updateTimeInMs = (totalUpdateTime / (double)fpsCounter) / 1000000.0f;
				measurements.Update(fpsCounter, updateTimeInMs);
				
				totalUpdateTime = 0.0;
				fpsCounter = 0;
				secondTracker = 0.0;
				totalSecondTracker++;

				if (totalSecondTracker >= NR_OF_SEC_TO_MEASURE) {
					measurements.SaveCurrentDataToFile();
					updateLogic = false;
				}
			}
		}

		//Update camera
		scene.GetCamera()->Update(2.0f, 40.0f, deltaTime, inputManager);

		//Render
		renderer.Render(scene);
		renderer.Present();
	}	

	delete inputManager;
	inputManager = nullptr;

	return 0;
}

HWND CreateShowWindow(int windowWidth, int windowHeight, InputManager* inputManagerPtr) {
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProcedure;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	//Create the window.
	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Boids",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth,
		windowHeight, NULL, NULL, wc.hInstance, NULL);


	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, 5);

	return hwnd;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hwnd);
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}