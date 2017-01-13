#include "GLShader.h"

#include<fstream>
#include <iostream>
#include <vector>


GLShader::GLShader()
{
}


GLShader::~GLShader()
{
	glDeleteProgram(m_Program);
}

bool GLShader::LoadShader(const std::string& a_VFilePath, const std::string& a_FFilePath)
{
	bool success = true;
	std::ifstream inV(a_VFilePath);
	std::ifstream inF(a_FFilePath);
	std::string contents((std::istreambuf_iterator<char>(inV)), std::istreambuf_iterator<char>());
	char errorLog[512];
	
	GLchar* source = (GLchar*)contents.c_str();
	GLuint shaderV = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderV, 1, &source, NULL);
	glCompileShader(shaderV);

	GLint succes = 0;
	glGetShaderiv(shaderV, GL_COMPILE_STATUS, &succes);
	
	if (succes == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderV, GL_INFO_LOG_LENGTH, &maxLength);

		glGetShaderInfoLog(shaderV, maxLength, &maxLength, errorLog);
		std::cout << errorLog << std::endl;
		success = false;
	}
	
	contents = std::string((std::istreambuf_iterator<char>(inF)), std::istreambuf_iterator<char>());

	source = (GLchar*)contents.c_str();
	
	GLuint shaderF = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderF, 1, &source, NULL);
	glCompileShader(shaderF);

	succes = 0;
	glGetShaderiv(shaderF, GL_COMPILE_STATUS, &succes);
	
	if (succes == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderF, GL_INFO_LOG_LENGTH, &maxLength);

		
		glGetShaderInfoLog(shaderF, maxLength, &maxLength, errorLog);
		std::cout << errorLog << std::endl;
		success = false;

	}
	

	m_Program = glCreateProgram();
	glAttachShader(m_Program, shaderV);
	glAttachShader(m_Program, shaderF);
	glLinkProgram(m_Program);

	glGetProgramiv(m_Program, GL_LINK_STATUS, &succes);
	
	if (succes == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &maxLength);
		
		glGetProgramInfoLog(m_Program, maxLength, &maxLength, errorLog);
		std::cout << errorLog << std::endl;
		success = false;
	}


	glDeleteShader(shaderV);
	glDeleteShader(shaderF);

	inV.close();
	inF.close();

	return true;

}

void GLShader::UseProgram()
{
	glUseProgram(m_Program);
}