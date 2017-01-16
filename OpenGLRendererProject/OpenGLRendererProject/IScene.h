#pragma once

#include "stdafx.h"
#include <vector>

#include "Light.h"
#include "Input.h"
#include "Camera.h"

namespace Primitive{

// Scene is a representation of all data to be rendered
// Lights, Objects, Particle systems all entail an item that should be rendered or that affect the rendering pipeline
class Camera;

// Interface for scene
class IScene
{
public:
	IScene();
	virtual ~IScene() = default;

	// Get deltatime and input every frame
	virtual void Update(float aDT, std::shared_ptr<Input> aInput) = 0;
	virtual void OnMouseMove(std::shared_ptr<Input> aInput) = 0;

	virtual void Init() = 0;

	const std::vector<Light>& GetAllLights();
	const std::shared_ptr<Camera> GetCamera();
	// Maybe protected destroy? To clear scenes in the destructor
	// void Destroy();
protected:
	std::vector<Light> mLights;

	// Only support one camera for now
	std::shared_ptr<Camera> mCamera;
};

}
