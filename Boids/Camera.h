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

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	
	//Helper functions
	glm::mat4 CalculateViewMatrix() const;
	glm::mat4 CalculateWorldMatrix() const;

public:
	//Constructors and deconstructors
	Camera();
	Camera(float fieldOfView, int width, int height);
	~Camera();

	//Getters and setters
	glm::vec3 GetPosition() const;
	glm::vec3 GetTarget() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void SetPosition(const glm::vec3 &position);
	void SetTarget(const glm::vec3 &target);

	//Functions
	void Update(float moveSpeed, float rotiationSpeed, float deltaTime, InputManager* inputManager);
	void Yaw(float rotation);
	void Pitch(float rotation);
};