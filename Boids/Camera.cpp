#include "Camera.h"

//Constructors and deconstructors
Camera::Camera() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->target = glm::vec3(0.0, 0.0, 1.0);
	this->up = glm::vec3(1.0, 0.0, 0.0);
	this->right = glm::vec3(0.0, 1.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->fieldOfView = 90.0f;
}

Camera::Camera(float fieldOfView) {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->target = glm::vec3(0.0, 0.0, 1.0);
	this->up = glm::vec3(1.0, 0.0, 0.0);
	this->right = glm::vec3(0.0, 1.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->fieldOfView = fieldOfView;
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

void Camera::SetPosition(const glm::vec3 &position) {
	this->position = position;
}

void Camera::SetTarget(const glm::vec3 &target) {
	this->target = target;
}

//Functions
void Camera::Update(float moveSpeed, float rotationSpeed, float deltaTime, InputManager* inputManager) {
	//TODO: Update camera
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