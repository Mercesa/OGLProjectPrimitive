// OpenGLRendererProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>


#include <glm\common.hpp>
#include <glm\gtx\common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "ShaderProgram.h"
#include "MeshData.h"
#include "GLMesh.h"
#include "Camera.h"
#include "Input.h"
#include "EngineTimer.h"
#include "ModelLoader.h"
#include "GLTextureLoader.h"
#include "Light.h"

using namespace Primitive;

// Shaders
static const int16_t sScreenWidth = 1024; 
static const int16_t sScreenHeight = 768;

std::vector<Light> gLights;
void BindLights(uint32_t shaderProgram)
{
	short int loopSize = 10;

	Light l;
	l.position = glm::vec3(0.0f, 1.0f, 0.0f);
	l.diffuseColor = glm::vec3(0.0f, 1.0f, 0.0f);
	l.specularColor = glm::vec3(0.0f, 0.0f, 0.0f);

	std::string shaderString = "Lights[" + std::to_string(0) + "].";
	GLuint tShaderProg = shaderProgram;

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

	//for (unsigned int i = 0; i < loopSize; i++)
	//{
	//	//TransformComp *tc = dynamic_cast<GameObject*>(LightComponent::ms_LightRegistry[i]->GetParentObject())->GetComponent<TransformComp>();
	//	//LightComponent *lc = LightComponent::ms_LightRegistry[i];
	//
	//	std::string shaderString = "lights[" + std::to_string(i) + "].";
	//	GLuint tShaderProg = shaderProgram;
	//
	//	GLuint lightDiffuse = glGetUniformLocation(tShaderProg, (shaderString + "diffuse").c_str());
	//	GLuint lightSpecular = glGetUniformLocation(tShaderProg, (shaderString + "specular").c_str());
	//	GLuint lightAmbient = glGetUniformLocation(tShaderProg, (shaderString + "ambient").c_str());
	//	GLuint lightPosition = glGetUniformLocation(tShaderProg, (shaderString + "position").c_str());
	//
	//
	//	//glUniform1i(glGetUniformLocation(tShaderProg, (shaderString + "used").c_str()), true);
	//	glUniform3f(lightDiffuse, lc->GetColor().x, lc->GetColor().y, lc->GetColor().z);
	//	glUniform3f(lightSpecular, lc->GetColor().x, lc->GetColor().y, lc->GetColor().z);
	//	//glUniform3f(lightAmbient, demo->lights[i]->ambient.x, demo->lights[i]->ambient.y, demo->lights[i]->ambient.z);
	//	glUniform3f(lightPosition, tc->m_Position.x, tc->m_Position.y, tc->m_Position.z);
	//}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, sScreenWidth, sScreenHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();
	
	glEnable(GL_MULTISAMPLE);

	SDL_GL_SetSwapInterval(1);

	// Define the viewport dimensions
	glViewport(0, 0, sScreenWidth, sScreenHeight);


	glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float)sScreenWidth / (float)sScreenHeight, 0.1f, 1000.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	auto basicVertexShader = std::make_unique<Shader>();
	basicVertexShader->LoadShader("Shaders\\VertexShaderBasicPosition.vert", eSHADER_VERTEX);

	auto basicFragmentShader = std::make_unique<Shader>();
	basicFragmentShader->LoadShader("Shaders\\FragmentShaderBasicPosition.frag", eSHADER_FRAGMENT);

	std::shared_ptr<ShaderProgram> mProgram = std::make_shared<ShaderProgram>();
	mProgram->AddShader(basicVertexShader.get());
	mProgram->AddShader(basicFragmentShader.get());
	mProgram->CompileProgram();

	auto lightVertexShader = std::make_unique<Shader>();
	lightVertexShader->LoadShader("Shaders\\VertexShaderBasicPosition.vert", eSHADER_VERTEX);

	auto lightFragmentShader = std::make_unique<Shader>();
	lightFragmentShader->LoadShader("Shaders\\FragmentShaderBasicPosition.frag", eSHADER_FRAGMENT);

	std::shared_ptr<ShaderProgram> mProgramLight = std::make_shared<ShaderProgram>();
	mProgram->AddShader(lightVertexShader.get());
	mProgram->AddShader(lightFragmentShader.get());
	mProgram->CompileProgram();


	MeshData data;	

	std::unique_ptr<ModelLoader> modLoader = std::make_unique<ModelLoader>();
	std::unique_ptr<GLTextureLoader> texLoader = std::make_unique<GLTextureLoader>();
	std::unique_ptr<Input> input = std::make_unique<Input>();
	auto cam = std::make_unique<Camera>();
	auto engTimer = std::make_unique<EngineTimer>();


	modLoader->LoadModel("Models\\Sponza\\Sponza.obj");

	glm::mat4x4 modelMatrix = glm::mat4x4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));

	std::vector<std::shared_ptr<GLMesh>> mMeshesToBeRendered;
	for (auto e : modLoader->GetMeshesToBeProcessed())
	{
		auto tGLMesh = std::make_shared<GLMesh>(e);
		
		tGLMesh->textureID = texLoader->LoadTexture(e.materialData.mTextures[0].mTextureFilePath);
		mMeshesToBeRendered.push_back(tGLMesh);
	}

	modLoader->ClearProcessedMeshes();

	SDL_Event evt;
	
	bool quit = false;
	while (!quit)
	{
		//engTimer->Start();
		engTimer->Update();

		bool mouseMove = false;

		// SDL event loop, click the window cross to kill the window and end the engine loop

		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_KEYDOWN:

				if (evt.key.keysym.sym == SDLK_ESCAPE)
					quit = true;

				input->KeyDown(evt.key.keysym.sym);
				break;

			case SDL_KEYUP:
				input->KeyUp(evt.key.keysym.sym);
				break;

			case SDL_MOUSEMOTION:
				cam->MoveWithMouse(evt.motion.x, evt.motion.y);
				mouseMove = true;
				break;

			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		if (input->GetKey(Input::KEYS::A))
		{
			cam->Strafe(engTimer->GetDeltaTime(), cameraMovement::CAM_LEFT);
		}

		if (input->GetKey(Input::KEYS::W))
		{
			cam->Strafe(engTimer->GetDeltaTime(), cameraMovement::CAM_UP);
		}

		if (input->GetKey(Input::KEYS::S))
		{
			cam->Strafe(engTimer->GetDeltaTime(), cameraMovement::CAM_DOWN);
		}

		if (input->GetKey(Input::KEYS::D))
		{
			cam->Strafe(engTimer->GetDeltaTime(), cameraMovement::CAM_RIGHT);
		}

		input->Update();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);

		// Draw our first triangle
		glUseProgram(mProgram->GetProgram());
		BindLights(mProgram->GetProgram());
		glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
		glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(cam->GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

		for (auto e : mMeshesToBeRendered)
		{
			glUniform1i(glGetUniformLocation(mProgram->GetProgram(), "diffuseTexture"), 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, e->textureID);

			glBindVertexArray(e->mVAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawElements(GL_TRIANGLES, e->mAmountOfIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}


		SDL_GL_SwapWindow(window);

		//return 0;
	}

	// Properly de-allocate all resources once they've outlived their purpose
	
	SDL_DestroyWindow(window);

	return 0;
}

