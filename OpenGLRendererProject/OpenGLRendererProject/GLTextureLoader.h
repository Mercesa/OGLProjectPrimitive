#pragma once

#include <string>

namespace Primitive
{

class GLTextureLoader
{
public:
	GLTextureLoader();
	~GLTextureLoader();

	uint32_t LoadTexture(const std::string& aString);
};


}
