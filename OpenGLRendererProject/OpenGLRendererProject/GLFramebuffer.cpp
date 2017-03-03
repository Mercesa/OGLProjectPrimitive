#include "GLFramebuffer.h"

#include <GL\glew.h>
#include <iostream>

using namespace Primitive;

GLFramebuffer::GLFramebuffer(uint32_t aFBOWidth, uint32_t aFBOHeight)
{
	
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, aFBOWidth, aFBOHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, aFBOWidth, aFBOHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	
	// - Albedo color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aFBOWidth, aFBOHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// - Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aFBOWidth, aFBOHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "GBuffer Framebuffer not complete!" << std::endl;


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


GLFramebuffer::~GLFramebuffer()
{
	// Perform some basic cleanup
	glDeleteFramebuffers(1, &this->gBuffer);
	GLuint textures[] = { gPosition, gNormal, gAlbedo };
	glDeleteTextures(3, textures);
	glDeleteRenderbuffers(1, &rboDepth);
}


void GLFramebuffer::PrintFramebufferLimits() {

	int res;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
	printf("Max Color Attachments: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &res);
	printf("Max Framebuffer Width: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &res);
	printf("Max Framebuffer Height: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
	printf("Max Framebuffer Samples: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
	printf("Max Framebuffer Layers: %d\n", res);

}