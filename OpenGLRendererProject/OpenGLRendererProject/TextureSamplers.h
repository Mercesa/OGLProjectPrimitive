#pragma once

#include "stdafx.h"
#include <GL\glew.h>

struct Sampler2D
{
	Sampler2D(int32_t aMinFilter, int32_t aMagFilter, int32_t aWrapS, int32_t aWrapT) : minFilter(aMinFilter), magFilter(aMagFilter), wrapS(aWrapS), wrapT(aWrapT) {}

	int32_t minFilter = 0;
	int32_t magFilter = 0;
	int32_t wrapS = 0;
	int32_t wrapT = 0;
};


const Sampler2D nearEdgeSampler = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };