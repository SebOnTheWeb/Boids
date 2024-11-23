#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>

#include <windows.h>
#include <d3d11.h>

#include "Simulation.h"
#include "Constants.h"

#pragma comment(lib, "d3d11.lib")

HWND CreateShowWindow(int windowWidth, int windowHeight);
HWND CreateHWND(int windowWidth, int windowHeight);
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool ShouldRun(MSG msg);

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	bool isRunning = true;

	HWND hwnd = CreateShowWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	Simulation simulation = Simulation(hwnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (isRunning) {
		simulation.Update();
		isRunning = ShouldRun(msg);
	}	

	return 0;
}

HWND CreateShowWindow(int windowWidth, int windowHeight) {
	HWND hwnd = CreateHWND(windowWidth, windowHeight);

	ShowWindow(hwnd, 5);

	return hwnd;
}

HWND CreateHWND(int windowWidth, int windowHeight) {
	const wchar_t CLASS_NAME[] = L"Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProcedure;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Boids",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth,
		windowHeight, NULL, NULL, wc.hInstance, NULL);


	if (hwnd == NULL) {
		return 0;
	}

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

bool ShouldRun(MSG msg) {
	bool shouldRun = true;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) {
			shouldRun = false;
		}
	}

	return shouldRun;
}