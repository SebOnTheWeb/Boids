#pragma once

#include "Scene.h"
#include "Renderer.h"

class BoidLogic {
public:
	BoidLogic();
	~BoidLogic();

	virtual void Update(Scene* scene, float deltaTime) = 0;
};