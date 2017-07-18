#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include "InputManager.h"

class Camera {
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 right;
	float yaw;
	float pitch;
	float fieldOfView;
	// TODO: Matrices

public:
	//Constructors and deconstructors
	Camera();
	Camera(float fieldOfView);
	~Camera();

	//Getters and setters
	glm::vec3 GetPosition() const;
	glm::vec3 GetTarget() const;
	void SetPosition(const glm::vec3 &position);
	void SetTarget(const glm::vec3 &target);
	// TODO: Rest of camera functions

	//Functions
	void Update(float moveSpeed, float rotiationSpeed, float deltaTime, InputManager* inputManager);
	void Yaw(float rotation);
	void Pitch(float rotation);
};