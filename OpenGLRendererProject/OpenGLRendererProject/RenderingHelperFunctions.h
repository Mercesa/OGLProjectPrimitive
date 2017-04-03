#pragma once


#include "stdafx.h"
#include "IScene.h"

static const uint16_t MAX_LIGHTS = 256;



static inline void BindLights(uint32_t shaderProgram, IScene* const aScenePtr)
{
	std::vector<Light> tLights = aScenePtr->GetAllLights();
	// Assure we don't go over the max
	uint16_t loopAmount = tLights.size() < MAX_LIGHTS ? tLights.size() : MAX_LIGHTS;
	GLuint tShaderProg = shaderProgram;
	GLuint amountOfLightsULoc = glGetUniformLocation(tShaderProg, "amountOfLights");

	glUniform1i(amountOfLightsULoc, loopAmount);

	for (unsigned int i = 0; i < loopAmount; i++)
	{
		std::cout << loopAmount << std::endl;
		Light& l = tLights[i];
		std::string shaderString = "Lights[" + std::to_string(i) + "].";

		GLuint lightDiffuse = glGetUniformLocation(tShaderProg, (shaderString + "DiffuseColor").c_str());
		GLuint lightSpecular = glGetUniformLocation(tShaderProg, (shaderString + "SpecularColor").c_str());
		GLuint lightAmbient = glGetUniformLocation(tShaderProg, (shaderString + "ambient").c_str());
		GLuint lightPosition = glGetUniformLocation(tShaderProg, (shaderString + "position").c_str());

		//glUniform1i(glGetUniformLocation(tShaderProg, (shaderString + "used").c_str()), true);
		glUniform3f(lightDiffuse, l.diffuseColor.r, l.diffuseColor.g, l.diffuseColor.b);
		glUniform3f(lightSpecular, l.specularColor.r, l.specularColor.g, l.specularColor.b);
		//glUniform3f(lightAmbient, demo->lights[i]->ambient.x, demo->lights[i]->ambient.y, demo->lights[i]->ambient.z);
		glUniform3f(lightPosition, l.position.x, l.position.y, l.position.z);
		// dont query all the lights
	}
}


static inline void RenderLights(GLMesh* const aMesh, uint32_t aProgram, IScene* const aScenePtr, glm::mat4 aProjection)
{
	glUseProgram(aProgram);
	glm::mat4x4 modelMatrix;

	for (auto e : aScenePtr->GetAllLights())
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, e.position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		glUniformMatrix4fv(glGetUniformLocation(aProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(aProgram, "projection"), 1, GL_FALSE, glm::value_ptr(aProjection));
		glUniformMatrix4fv(glGetUniformLocation(aProgram, "view"), 1, GL_FALSE, glm::value_ptr(aScenePtr->GetCamera()->GetViewMatrix()));

		GLuint lightDiffuse = glGetUniformLocation(aProgram, "lightColor");
		glUniform3f(lightDiffuse, e.diffuseColor.r, e.diffuseColor.g, e.diffuseColor.b);

		glBindVertexArray(aMesh->mVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, aMesh->mAmountOfIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glUseProgram(0);
}

// Taken from learnopengl.com, could've done it myself but too lazy right now
GLuint quadVAO = 0;
GLuint quadVBO;
static inline void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

