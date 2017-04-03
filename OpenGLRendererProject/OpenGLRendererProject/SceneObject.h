#pragma once


#include <glm\common.hpp>
#include <glm\gtx\common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "GLMesh.h"
// Scene object has transform, and has meshes. Normally I would opt for a more elaborate approach, but for now it is just data that will be transformed.

namespace Primitive
{
class Mesh;

class SceneObject
{
public:
	SceneObject();
	~SceneObject();


	glm::mat4 mTransform;
	std::vector<std::shared_ptr<GLMesh>> mMeshes;
};
};
