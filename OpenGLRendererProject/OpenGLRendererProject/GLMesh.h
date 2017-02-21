#pragma once

#include "stdafx.h"


#include "GLTexture.h"

namespace Primitive
{

	// GLMesh keeps OGL specific data

struct MeshData;

class GLMesh
{
public:
	GLMesh(const MeshData& aData);
	~GLMesh();

	uint32_t mVBO, mVAO, mEBO;
	uint32_t mAmountOfIndices;

	std::shared_ptr<GLTexture> mTexture;

};

}