#include "ShaderProgram.h"


#include <GL\glew.h>
#include <assert.h>

#include "Shader.h"

using namespace Primitive;

ShaderProgram::ShaderProgram() : mVertexShader(nullptr), mFragmentShader(nullptr), mGeometryShader(nullptr), mComputeShader(nullptr), hasCompiled(false)
{
}


ShaderProgram::~ShaderProgram()
{
	// If a shader has been compiled then it should be deleted.
	if(hasCompiled)
	glDeleteProgram(mShaderProgramID);
}


void ShaderProgram::UseProgram()
{
	assert(this->hasCompiled == true);
	glUseProgram(mShaderProgramID);
}


void ShaderProgram::CompileProgram(bool isCompute)
{
	// Make sure this program is not compiled yet
	assert(this->hasCompiled != true && "Shader already has compiled");
	if (this->hasCompiled == true)
	{
		std::cout << "ERROR:SHADER::PROGRAM::COMPILE::COMPILING FAILED\n";
		return;
	}

	GLint success;
	char errorlog[1024];
	mShaderProgramID = glCreateProgram();
	
	if (isCompute)
	{
		assert(mComputeShader != nullptr);
		
		glAttachShader(mShaderProgramID, mComputeShader->GetShaderID());
	}

	else
	{
		assert(mVertexShader != nullptr);
		assert(mFragmentShader != nullptr);

		glAttachShader(mShaderProgramID, mVertexShader->GetShaderID());
		glAttachShader(mShaderProgramID, mFragmentShader->GetShaderID());
	}

	glLinkProgram(mShaderProgramID);

	glGetProgramiv(mShaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mShaderProgramID, 512, NULL, errorlog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorlog << std::endl;
	}

	hasCompiled = true;
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

		case Shader_Types::eSHADER_COMPUTE:
			mComputeShader = aShader;
			break;

		case Shader_Types::eSHADER_UNKNOWN:
			std::cout << "ERROR::SHADERPROGRAM::ADDSHADER::UNKNOWN_SHADERTYPE\n";
			break;
	}
}