#pragma once

#include "Renderer.h"
#include "Scene.h"
#include "BoidLogicCPUSingle.h"
#include "BoidLogicCPUMulti.h"
#include "BoidLogicGPU.h"
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

	InputManager* inputManager;
	Renderer* renderer;
	Scene* scene;
	BoidLogic* boidLogic;

	Measurements* measurements;
	Timer mainTimer;
	Timer boidLogicTimer;

	void ToggleUpdateLogicIfSpaceWasPressed();
	void UpdateDeltaTime();
	void UpdateMeasurements();
	void UpdateRenderLogic();
	void SaveMeasurementsForSecond();
	void ResetMetricsForSecond();
	bool IsTestDone();

public:
	Simulation(HWND hwnd);
	~Simulation();

	void Update();
};