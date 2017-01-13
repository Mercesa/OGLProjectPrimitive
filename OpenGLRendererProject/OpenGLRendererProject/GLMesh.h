#pragma once

#include "stdafx.h"


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

	uint32_t textureID;

};

}