#pragma once

#include "stdafx.h"

namespace Primitive{


// Wrapper for openGL textures, we want to destroy textures when they go out of scope
// This wrapper takes care of deletion, 
class GLTexture
{
public:
	GLTexture(uint32_t aTextureID);
	~GLTexture();

	uint32_t GetTextureID();

private:
	uint32_t mTextureID;
};
};
