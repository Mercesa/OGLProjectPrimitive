#include "GLTexture.h"

#include <GL\glew.h>

using namespace Primitive;

GLTexture::GLTexture(uint32_t aTextureID) : mTextureID(aTextureID)
{
}


GLTexture::~GLTexture()
{
	glDeleteTextures(1, &mTextureID);
}


uint32_t GLTexture::GetTextureID()
{
	return mTextureID;
}