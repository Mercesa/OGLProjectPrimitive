#pragma once

#include <string>

#include "stdafx.h"

namespace Primitive{

enum Shader_Types
{
	eSHADER_VERTEX,
	eSHADER_FRAGMENT,
	eSHADER_GEOMETRY,
	eSHADER_TESSELATION_CONTROL,
	eSHADER_TESSELATION_EVELUATION,
	eSHADER_COMPUTE,
};


class Shader
{
	// Shader has a type
public:
	Shader();
	~Shader();

	// Load your shader with the type
	bool LoadShader(const std::string& aFilePath, Shader_Types aType);
	uint32_t GetShaderID() { return mShader; }
	Shader_Types GetShaderType() { return mShaderType; }

protected:
	std::string mShaderFileLocation;
	Shader_Types mShaderType;
	uint32_t mShader;
};
}
