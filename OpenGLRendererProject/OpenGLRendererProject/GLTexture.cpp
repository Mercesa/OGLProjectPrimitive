#include "GLTexture.h"

#include <GL\glew.h>

using namespace Primitive;

GLTexture::GLTexture(GLint aLevel, GLint internalFormat, GLint aScreenWidth, GLint aScreenHeight, GLint aBorder, GLint aExternalFormat, GLenum aType, void* pixels)
{
	glGenTextures(1, &mTextureID);

	glTexImage2D(GL_TEXTURE_2D, aLevel, internalFormat, aScreenWidth, aScreenHeight, aBorder, aExternalFormat, aType, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::GLTexture(GLuint aTexture) : mTextureID(aTexture)
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