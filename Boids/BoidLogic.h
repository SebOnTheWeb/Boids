#pragma once

#include "Scene.h"
#include "Renderer.h"

class BoidLogic {
private:
	Renderer* rendererPtr;

public:
	BoidLogic(Renderer* rendererPtr);
	~BoidLogic();

	virtual void Update(Scene* scene, float deltaTime) = 0;
};