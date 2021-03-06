#pragma once

#include "stdafx.h" 


namespace Primitive{

class Shader;

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void UseProgram();
	uint32_t GetProgram() { return mShaderProgramID; }

	void CompileProgram(bool isCompute = false);

	void AddShader(Shader* const aShader);

private:
	// Just support these shaders for now
	Shader* mVertexShader;
	Shader* mFragmentShader;
	Shader* mGeometryShader;
	Shader* mComputeShader;

	uint32_t mShaderProgramID;

	bool hasCompiled;
};

}