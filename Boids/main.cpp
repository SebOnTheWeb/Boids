#include <windows.h>
#include <d3d11.h>

#include "Renderer.h"
#include "Scene.h"
#include "BoidLogicHandler.h"
#include "InputManager.h"

#pragma comment(lib, "d3d11.lib")


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateShowWindow(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow, int windowWidth, int windowHeight);

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;

	HWND hwnd = CreateShowWindow(hInstance, pCmdLine, nCmdShow, windowWidth, windowHeight);

	//Init classes
	Renderer renderer = Renderer(hwnd, hInstance, windowWidth, windowHeight);
	InputManager inputManager = InputManager(&hInstance, &hwnd);
	Scene scene = Scene(&renderer);
	BoidLogicHandler boidLogic = BoidLogicHandler(&renderer);


	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Main loop
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		float deltaTime = 0.0f;

		boidLogic.SingleThreadUpdate(&scene, deltaTime);
		//boidLogic.MultiThreadUpdate(scene, deltaTime);
		//boidLogic.GPUUpdate(scene, deltaTime);
		inputManager.Update();
		scene.GetCamera().Update(0.01f, 0.001f, deltaTime, &inputManager);

		renderer.Render(scene);
		renderer.Present();
	}

	return 0;
}

HWND CreateShowWindow(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow, int windowWidth, int windowHeight) {
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProcedure;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	//Create the window.
	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Window Display Name",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth,
		windowHeight, NULL, NULL, hInstance, NULL);


	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	return hwnd;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}