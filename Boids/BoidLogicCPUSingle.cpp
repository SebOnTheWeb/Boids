#pragma once

#include "BoidLogicCPUSingle.h"

BoidLogicCPUSingle::BoidLogicCPUSingle(Scene* scenePtr): BoidLogicCPU(scenePtr) {

}

BoidLogicCPUSingle::~BoidLogicCPUSingle() {

}

void BoidLogicCPUSingle::Update(Scene* scene, float deltaTime) {
	scene->SwitchCurrentAndPreviousBoids();
	Boid* allBoidsPrevious = scene->GetAllBoidsPrevious();
	Boid* allBoids = scene->GetAllBoids();

	for (int i = 0; i < NR_OF_BOIDS; i++) {
		UpdateBoid(allBoidsPrevious, allBoids, i, deltaTime);
	}

	scene->GetBoidBuffer(0)->SetData(scene->GetAllBoids(), sizeof(Boid) * NR_OF_BOIDS);
}