#pragma once

#include "stdafx.h"

#include <GL\glew.h>
namespace Primitive{


// Wrapper for openGL textures, we want to destroy textures when they go out of scope
// This wrapper takes care of deletion, 
class GLTexture
{
public:
	// Some parameters to create our textures with
	GLTexture(GLint aLevel, GLint internalFormat, GLint aScreenWidth, GLint aScreenHeight, GLint aBorder, GLint aExternalFormat, GLenum aType, void* pixels);

	// Constructor for if we already initialized a texture
	GLTexture(GLuint aTexture);

	~GLTexture();

	uint32_t GetTextureID();

private:
	uint32_t mTextureID;
};
};
