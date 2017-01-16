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
#include "FirstTestScene.h"

using namespace Primitive;

// Shaders
static const int16_t sScreenWidth = 1024;
static const int16_t sScreenHeight = 768;

std::vector<Light> gLights;
static const uint16_t MAX_LIGHTS = 256;
std::unique_ptr<IScene> mScene = std::make_unique<FirstTestScene>();


void BindLights(uint32_t shaderProgram, IScene* const aScenePtr)
{
	std::vector<Light> tLights = aScenePtr->GetAllLights();
	// Assure we don't go over the max
	uint16_t loopAmount = tLights.size() < MAX_LIGHTS ? tLights.size() : MAX_LIGHTS;
	

	for (unsigned int i = 0; i < loopAmount; i++)
	{
		Light& l = tLights[i];
		std::string shaderString = "Lights[" + std::to_string(i) + "].";
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
	}

}

glm::mat4 Projection;
glm::mat4 View;

void RenderLights(GLMesh* const aMesh, uint32_t aProgram)
{
	glUseProgram(aProgram);
	glm::mat4x4 modelMatrix;

	for (auto e : mScene->GetAllLights())
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, e.position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		glUniformMatrix4fv(glGetUniformLocation(aProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(aProgram, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
		glUniformMatrix4fv(glGetUniformLocation(aProgram, "view"), 1, GL_FALSE, glm::value_ptr(mScene->GetCamera()->GetViewMatrix()));

		GLuint lightDiffuse = glGetUniformLocation(aProgram, "lightColor");		
		glUniform3f(lightDiffuse, e.diffuseColor.r, e.diffuseColor.g, e.diffuseColor.b);

		glBindVertexArray(aMesh->mVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, aMesh->mAmountOfIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glUseProgram(0);
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


	Projection = glm::perspective(glm::radians(90.0f), (float)sScreenWidth / (float)sScreenHeight, 0.1f, 1000.0f);
	View = glm::lookAt(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	auto basicVertexShader = std::make_unique<Shader>();
	basicVertexShader->LoadShader("Shaders\\VertexShaderBasicPosition.vert", eSHADER_VERTEX);

	auto basicFragmentShader = std::make_unique<Shader>();
	basicFragmentShader->LoadShader("Shaders\\FragmentShaderBasicPosition.frag", eSHADER_FRAGMENT);

	std::shared_ptr<ShaderProgram> mProgram = std::make_shared<ShaderProgram>();
	mProgram->AddShader(basicVertexShader.get());
	mProgram->AddShader(basicFragmentShader.get());
	mProgram->CompileProgram();

	auto lightVertexShader = std::make_unique<Shader>();
	lightVertexShader->LoadShader("Shaders\\VertexShaderLight.vert", eSHADER_VERTEX);

	auto lightFragmentShader = std::make_unique<Shader>();
	lightFragmentShader->LoadShader("Shaders\\FragmentShaderLight.frag", eSHADER_FRAGMENT);

	std::shared_ptr<ShaderProgram> mProgramLight = std::make_shared<ShaderProgram>();
	mProgramLight->AddShader(lightVertexShader.get());
	mProgramLight->AddShader(lightFragmentShader.get());
	mProgramLight->CompileProgram();


	MeshData data;	

	std::unique_ptr<ModelLoader> modLoader = std::make_unique<ModelLoader>();
	std::unique_ptr<GLTextureLoader> texLoader = std::make_unique<GLTextureLoader>();
	std::shared_ptr<Input> input = std::make_shared<Input>();
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



	
	std::shared_ptr<GLMesh> tMesh;;
	modLoader->LoadModel("Models\\Cube.obj");
	for (auto e : modLoader->GetMeshesToBeProcessed())
	{
		tMesh = std::make_shared<GLMesh>(e);
	}
	modLoader->ClearProcessedMeshes();



	SDL_Event evt;
	
	mScene->Init();
	bool quit = false;
	while (!quit)
	{
		//engTimer->Start();
		engTimer->Update();
		input->Update();

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
				input->MouseMove(evt.motion.x, evt.motion.y);
				mScene->OnMouseMove(input);
				mouseMove = true;
				break;

			case SDL_QUIT:
				quit = true;
				break;
			}
		}
		
		mScene->Update(engTimer->GetDeltaTime(), input);

		static bool usePhong = true;
		if (input->GetKeyDown(Primitive::Input::KEYS::SPACE))
		{
			usePhong = !usePhong;
			std::cout << "Use phong " << usePhong << std::endl;
		}
	
		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);

		// Draw our first triangle
		glUseProgram(mProgram->GetProgram());
		BindLights(mProgram->GetProgram(), mScene.get());
		glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
		glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(mScene->GetCamera()->GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1i(glGetUniformLocation(mProgram->GetProgram(), "usePhong"), (int)usePhong);

		glUniform3f(glGetUniformLocation(mProgram->GetProgram(), "cameraPosition"), mScene->GetCamera()->camPosition.x, mScene->GetCamera()->camPosition.y, mScene->GetCamera()->camPosition.z);


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

		glUseProgram(0);


		RenderLights(tMesh.get(), mProgramLight->GetProgram());
		SDL_GL_SwapWindow(window);

		//return 0;
	}

	// Properly de-allocate all resources once they've outlived their purpose
	
	SDL_DestroyWindow(window);

	return 0;
}

