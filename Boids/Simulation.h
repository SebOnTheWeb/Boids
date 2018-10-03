#pragma once

#include "Renderer.h"
#include "Scene.h"
#include "BoidLogicHandler.h"
#include "InputManager.h"
#include "Measurements.h"
#include "Timer.h"
#include "Constants.h"

class Simulation {
private:
	bool updateLogic;
	float deltaTime;

	int totalSecondTracker;
	double secondTracker;
	int fpsCounter;
	double totalUpdateTimeInMs;

	Renderer* renderer;
	Scene* scene;
	BoidLogicHandler* boidLogic;

	Measurements* measurements;
	Timer mainTimer;
	Timer boidLogicTimer;

	InputManager* inputManager;

	void ToggleUpdateLogicIfSpaceWasPressed();
	void UpdateDeltaTime();
	void UpdateMeasurements();
	void UpdateRenderLogic();
	void SaveMeasurementsForSecond();
	void ResetMetricsForSecond();
	bool IsTestDone();

public:
	Simulation(InputManager* inputManager, HWND hwnd);
	~Simulation();

	void Update();
};