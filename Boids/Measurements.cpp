#pragma once

#include "Measurements.h"

Measurements::Measurements() {
	this->fpsData = nullptr;
	this->updateTimeDataInMs = nullptr;
	this->maxDataElements = 0;
	this->nrOfDataElements = 0;
}

Measurements::Measurements(int maxDataElements) {
	this->fpsData = new int[maxDataElements];
	this->updateTimeDataInMs = new double[maxDataElements];
	this->maxDataElements = maxDataElements;
	this->nrOfDataElements = 0;
}

Measurements::~Measurements() {
	delete this->fpsData;
	fpsData = nullptr;

	delete this->updateTimeDataInMs;
	updateTimeDataInMs = nullptr;
}

void Measurements::Update(int fps, double updateTimeInMs) {
	this->fpsData[this->nrOfDataElements] = fps;
	this->updateTimeDataInMs[this->nrOfDataElements] = updateTimeInMs;
	this->nrOfDataElements++;
}

void Measurements::SaveCurrentDataToFile() {
	SaveToFile(this->fpsData, "fps_single_size64_run1.csv");
	SaveToFile(this->updateTimeDataInMs, "update_single_size64_run1.csv");
}