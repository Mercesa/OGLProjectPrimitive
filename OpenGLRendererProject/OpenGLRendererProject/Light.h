#pragma once

#include <glm\vec3.hpp>

namespace Primitive
{

struct Light
{
	glm::vec3 position;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};

}
