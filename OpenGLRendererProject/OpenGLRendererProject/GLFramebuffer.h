#pragma once


#include "stdafx.h"

namespace Primitive
{

class GLFramebuffer
{
public:
	// When creating, it also generates an FBO buffer object with it
	GLFramebuffer(uint32_t aFBOWidth, uint32_t aFBOHeight);
	virtual ~GLFramebuffer();

	//bool CheckStatus();

	static void PrintFramebufferLimits();

	uint32_t gPosition, gNormal, gAlbedo;

	uint32_t gBuffer;
	uint32_t rboDepth;

};
};
