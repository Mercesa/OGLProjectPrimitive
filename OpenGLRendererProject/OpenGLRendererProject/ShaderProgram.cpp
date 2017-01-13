#include "ShaderProgram.h"


#include <GL\glew.h>
#include <assert.h>

#include "Shader.h"

using namespace Primitive;

ShaderProgram::ShaderProgram() : mVertexShader(nullptr), mFragmentShader(nullptr), mGeometryShader(nullptr)
{
}


ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::CompileProgram()
{
	GLint success;
	char errorlog[1024];

	mShaderProgramID = glCreateProgram();

	
	assert(mVertexShader != nullptr);
	assert(mFragmentShader != nullptr);

	glAttachShader(mShaderProgramID, mVertexShader->GetShaderID());
	glAttachShader(mShaderProgramID, mFragmentShader->GetShaderID());
	glLinkProgram(mShaderProgramID);
	// Check for linking errors
	glGetProgramiv(mShaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mShaderProgramID, 512, NULL, errorlog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorlog << std::endl;
	}
}

void ShaderProgram::AddShader(Shader* const aShader)
{
	switch (aShader->GetShaderType())
	{
		case Shader_Types::eSHADER_VERTEX:
			mVertexShader = aShader;
			break;

		case Shader_Types::eSHADER_FRAGMENT:
			mFragmentShader = aShader;
			break;

		case Shader_Types::eSHADER_GEOMETRY:
			mGeometryShader = aShader;
			break;
	}
}