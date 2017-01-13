#pragma once

#include <glm\common.hpp>


namespace Primitive{

enum cameraMovement{CAM_UP, CAM_DOWN, CAM_LEFT, CAM_RIGHT};

class Camera
{
public:
	Camera();
	~Camera();
	void Strafe(float aDT, cameraMovement camDirection);

	void MoveWithMouse(int aX, int aY);
	glm::mat4x4 GetViewMatrix();

	glm::vec3 camPosition;

private:
	float pitch = 0.0f;
	float yaw = 0.0f;
	glm::vec3 camForward;
};
}
