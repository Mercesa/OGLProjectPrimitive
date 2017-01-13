#pragma once

#include <iostream>
#include <string>

#include <GL\glew.h>
class GLShader
{
public:
	GLShader();
	~GLShader();

	bool LoadShader(const std::string& a_VFilePath, const std::string& a_FFilePath);
	
	GLuint GetProgram() { return m_Program; }
	void UseProgram();

private:
	GLuint m_Program;
};

