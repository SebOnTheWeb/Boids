#pragma once

#include "BoidLogicCPUMulti.h"

void BoidLogicCPUMulti::BoidThread(Scene* scene, int startIndex, int endIndex, float deltaTime) {
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	Boid* allBoids = scene->GetAllBoids();

	for (int i = startIndex; i < endIndex; i++) {
		UpdateBoid(allBoidsPrevious, allBoids, i, deltaTime);
	}
}

BoidLogicCPUMulti::BoidLogicCPUMulti(Scene* scenePtr) : BoidLogicCPU(scenePtr) {

}

BoidLogicCPUMulti::~BoidLogicCPUMulti() {

}

void BoidLogicCPUMulti::Update(Scene* scene, float deltaTime) {
	scene->SwitchCurrentAndPreviousBoids();
	const int THREADS = 8;
	std::thread threadPool[THREADS];

	int startIndex = 0;
	int endIndex = 0;

	for (int i = 0; i < THREADS; i++) {
		startIndex = i * (NR_OF_BOIDS / THREADS);
		endIndex = (i * (NR_OF_BOIDS / THREADS)) + (NR_OF_BOIDS / THREADS);
		threadPool[i] = std::thread(BoidThread, scene, startIndex, endIndex, deltaTime);
	}

	for (auto& th : threadPool) {
		th.join();
	}

	scene->GetBoidBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}