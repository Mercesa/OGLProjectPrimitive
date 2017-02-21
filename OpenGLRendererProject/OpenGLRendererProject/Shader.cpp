#include "Shader.h"

#include <assert.h>
#include <GL\glew.h>

#include "stdafx.h"

using namespace Primitive;

Shader::Shader() : hasCompiled(false), mShaderType(Shader_Types::eSHADER_UNKNOWN)
{
}


Shader::~Shader()
{
	glDeleteShader(mShader);
}

bool Shader::LoadShader(const std::string& aFilePath, Shader_Types aType)
{
	//assert(aShader != nullptr);

	int32_t shaderType = -1;

	switch (aType) 
	{
		case Shader_Types::eSHADER_VERTEX:
			shaderType = GL_VERTEX_SHADER;
			break;

		case Shader_Types::eSHADER_FRAGMENT:
			shaderType = GL_FRAGMENT_SHADER;
			break;
		
		case Shader_Types::eSHADER_GEOMETRY:
			shaderType = GL_GEOMETRY_SHADER;
			break;

		case Shader_Types::eSHADER_TESSELATION_CONTROL:
			shaderType = GL_TESS_CONTROL_SHADER;
			break;

		case Shader_Types::eSHADER_TESSELATION_EVELUATION:
			shaderType = GL_TESS_EVALUATION_SHADER;
			break;

		case Shader_Types::eSHADER_COMPUTE:
			shaderType = GL_COMPUTE_SHADER;
			break;
	}

	bool success = true;


	std::ifstream tInFile(aFilePath);
	std::string tContents((std::istreambuf_iterator<char>(tInFile)), std::istreambuf_iterator<char>());
	GLchar* source = (GLchar*)tContents.c_str();


	mShader = glCreateShader(shaderType);
	glShaderSource(mShader, 1, &source, NULL);
	glCompileShader(mShader);

	GLint succesGL = 0;
	char errorLog[1024];

	glGetShaderiv(mShader, GL_COMPILE_STATUS, &succesGL);

	if (succesGL == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &maxLength);

		glGetShaderInfoLog(mShader, maxLength, &maxLength, errorLog);
		
		// Provide a better way of logging errors		
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << errorLog << std::endl;

		// early out
		return false;
	}


	this->mShaderFileLocation = aFilePath;
	this->mShaderType = aType;

	return true;
}