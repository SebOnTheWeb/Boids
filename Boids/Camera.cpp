#include "Camera.h"

//Helper functions
glm::mat4 Camera::CalculateViewMatrix() const {
	glm::mat4 matrix = glm::transpose(glm::mat4(
		glm::vec4(this->right, 0.0f),
		glm::vec4(this->up, 0.0f),
		glm::vec4(this->target, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	));

	return matrix * glm::translate(glm::mat4(), -this->position);
}

//Constructors and deconstructors
Camera::Camera() {
	this->position = glm::vec3(0.0, 0.0, -1.0);
	this->target = glm::vec3(0.0, 0.0, 1.0);
	this->up = glm::vec3(0.0, 1.0, 0.0);
	this->right = glm::vec3(1.0, 0.0, 0.0);
	this->fieldOfView = 90.0f;
}

Camera::Camera(float fieldOfView, int width, int height) {
	this->position = glm::vec3(0.0, 1.0, -2.0);
	this->target = glm::vec3(0.0, 0.0, 1.0);
	this->up = glm::vec3(0.0, 1.0, 0.0);
	this->right = glm::vec3(1.0, 0.0, 0.0);
	this->fieldOfView = fieldOfView;

	this->projectionMatrix = glm::perspectiveFovLH(glm::radians(this->fieldOfView),
		(float)width,
		(float)height,
		0.1f,
		100.0f);
}

Camera::~Camera() {

}

//Getters and setters
glm::vec3 Camera::GetPosition() const {
	return this->position;
}

glm::vec3 Camera::GetTarget() const {
	return this->target;
}

glm::mat4 Camera::GetViewMatrix() const {
	return this->viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const {
	return this->projectionMatrix;
}

void Camera::SetPosition(const glm::vec3 &position) {
	this->position = position;
}

void Camera::SetTarget(const glm::vec3 &target) {
	this->target = target;
}

//Functions
void Camera::Update(float moveSpeed, float rotationSpeed, float deltaTime, InputManager* inputManager) {
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	//Double movement when shift is pressed
	if (inputManager->KeyPressed(VK_LSHIFT)) {
		moveSpeed *= 2;
		rotationSpeed *= 2;
	}

	//Update movement based on keys pressed
	if (inputManager->KeyPressed(0x41)) { // A
		movement -= this->right;
	}
	if (inputManager->KeyPressed(0x44)) { // D
		movement += this->right;
	}
	if (inputManager->KeyPressed(0x57)) { // W
		movement += this->target;
	}
	if (inputManager->KeyPressed(0x53)) { // S
		movement -= this->target;
	}

	//TODO: Fix camera roation being laggy
	//Update rotation based on mouse movement and keys pressed
	if (inputManager->KeyPressed(VK_RBUTTON)) {
		glm::vec2 deltaMove = inputManager->MouseDeltaMovement();
		rotation.x = deltaMove.x * deltaTime * rotationSpeed; 
		rotation.y = deltaMove.y * deltaTime * rotationSpeed;
	}
	if (inputManager->KeyPressed(0x51)) { // Q
		rotation.z = deltaTime * rotationSpeed;
	}
	if (inputManager->KeyPressed(0x45)) { // E
		rotation.z = deltaTime * -rotationSpeed;
	}
	
	this->position += movement * moveSpeed * deltaTime;
	
	Yaw(rotation.x);
	Pitch(rotation.y);
	Roll(rotation.z);

	this->viewMatrix = CalculateViewMatrix();
}

void Camera::Yaw(float rotation) {
	glm::quat quaternion = glm::angleAxis(glm::radians(rotation), up);
	this->target = glm::normalize(quaternion * target);
	this->right = glm::normalize(quaternion * right);
}

void Camera::Pitch(float rotation) {
	glm::quat quaternion = glm::angleAxis(glm::radians(rotation), right);
	this->target = glm::normalize(quaternion * target);
	this->up = glm::normalize(quaternion * up);
}

void Camera::Roll(float rotation) {
	glm::quat quaternion = glm::angleAxis(glm::radians(rotation), target);
	this->right = glm::normalize(quaternion * right);
	this->up = glm::normalize(quaternion * up);
}