#include "Camera.h"

#include "stdafx.h"

#include <glm\gtc\matrix_transform.hpp>

using namespace Primitive;

Camera::Camera() : camPosition(glm::vec3(0.0f, 0.0f, -2.0f)), yaw(0.0f), pitch(0.0f)
{
}


Camera::~Camera()
{
}

static float gSpeed = 6.0f;

void Camera::Strafe(float aDT, cameraMovement camDirection)
{
	switch (camDirection)
	{
	case cameraMovement::CAM_LEFT:
		camPosition -= glm::cross(this->camForward, glm::vec3(0.0f, 1.0f, 0.0f)) * aDT * gSpeed;
		break;

	case cameraMovement::CAM_RIGHT:
		camPosition += glm::cross(this->camForward, glm::vec3(0.0f, 1.0f, 0.0f)) * aDT * gSpeed;
		break;

	case cameraMovement::CAM_UP:
		camPosition += this->camForward * aDT * gSpeed;
		break;

	case cameraMovement::CAM_DOWN:
		camPosition -= this->camForward * aDT * gSpeed;
		break;
	}
}


glm::mat4x4 Camera::GetViewMatrix()
{
	return glm::lookAt(camPosition, camPosition + camForward, glm::vec3(0.0f, 1.0f, 0.0f));
}


void Camera::MoveWithMouse(int aX, int aY)
{
	static bool firstMouse = false;

	static float_t lastX;
	static float_t lastY;

	if (!firstMouse)
	{
		lastX = 1024/2.0f;
		lastY = 768/2.0f;
		firstMouse = true;
	}


	float xoffset = aX - lastX;
	float yoffset = lastY - aY;

	float sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	camForward.x = cosf(glm::radians(this->pitch)) * cosf(glm::radians(this->yaw));
	camForward.y = sinf(glm::radians(pitch));
	camForward.z = cosf(glm::radians(this->pitch)) * sinf(glm::radians(this->yaw));

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	} 
	
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	lastX = aX;
	lastY = aY;
}