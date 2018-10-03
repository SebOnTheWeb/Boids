#pragma once

#include "Simulation.h"

Simulation::Simulation(HWND hwnd) {
	this->updateLogic = false;
	this->deltaTime = 0.0f;

	//Init fps tracking
	this->totalSecondTracker = 0;
	this->secondTracker = 0.0;
	this->fpsCounter = 0;
	this->totalUpdateTimeInMs = 0.0;

	this->inputManager = new InputManager(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->renderer = new Renderer(hwnd, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->scene = new Scene(renderer);
	this->boidLogic = new BoidLogicHandler(renderer);

	this->measurements = new Measurements(NR_OF_SEC_TO_MEASURE);
	this->mainTimer = Timer();
	this->boidLogicTimer = Timer();

	//################## Logic init options #######################
	//boidLogic.InitCPULogic(&scene);
	boidLogic->InitGPULogic(scene);
	//#############################################################

	mainTimer.Start();
}

Simulation::~Simulation() {
	delete this->inputManager;
	this->inputManager = nullptr;

	delete this->renderer;
	this->renderer = nullptr;

	delete this->scene;
	this->scene = nullptr;

	delete this->boidLogic;
	this->boidLogic = nullptr;

	delete this->measurements;
	this->measurements = nullptr;
}

void Simulation::Update() {
	inputManager->Update();
	ToggleUpdateLogicIfSpaceWasPressed();

	UpdateDeltaTime();

	if (updateLogic) {
		boidLogicTimer.Start();
		//################## Logic update options #####################
		//boidLogic.SingleThreadUpdate(&scene, deltaTime);
		//boidLogic.MultiThreadUpdate(&scene, deltaTime);
		boidLogic->GPUUpdate(scene, deltaTime);
		//#############################################################
		boidLogicTimer.Stop();

		UpdateMeasurements();
	}

	UpdateRenderLogic();
}

void Simulation::ToggleUpdateLogicIfSpaceWasPressed() {
	if (inputManager->SpaceDown()) {
		if (updateLogic)
			this->updateLogic = false;
		else
			this->updateLogic = true;
	}
}

void Simulation::UpdateDeltaTime() {
	mainTimer.Stop();
	this->deltaTime = mainTimer.GetDeltaTimeInSeconds();
	mainTimer.Start();
}

void Simulation::UpdateMeasurements() {
	totalUpdateTimeInMs += boidLogicTimer.GetDeltaTimeInMilliSeconds();

	this->fpsCounter++;
	this->secondTracker += deltaTime;

	if (secondTracker >= 1.0) {
		SaveMeasurementsForSecond();
		ResetMetricsForSecond();

		totalSecondTracker++;
		if (IsTestDone()) {
			measurements->SaveCurrentDataToFile();
			this->updateLogic = false;
		}
	}
}

void Simulation::UpdateRenderLogic() {
	scene->GetCamera()->Update(2.0f, 40.0f, deltaTime, inputManager);
	renderer->Render(*scene);
	renderer->Present();
}

void Simulation::SaveMeasurementsForSecond() {
	double updateTimeInMs = (totalUpdateTimeInMs / (double)fpsCounter);
	measurements->Update(fpsCounter, updateTimeInMs);
}

void Simulation::ResetMetricsForSecond() {
	this->totalUpdateTimeInMs = 0.0;
	this->fpsCounter = 0;
	this->secondTracker = 0.0;
}

bool Simulation::IsTestDone() {
	return totalSecondTracker >= NR_OF_SEC_TO_MEASURE;
}