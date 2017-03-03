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
#include "GLTimer.h"
#include "Camera.h"
#include "Input.h"
#include "EngineTimer.h"
#include "ModelLoader.h"
#include "GLTextureLoader.h"
#include "Light.h"
#include "FirstTestScene.h"
#include "GLTexture.h"
#include "GLFramebuffer.h"
#include "BloomEnvironment.h"

GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
{
	return a + f * (b - a);
}


using namespace Primitive;


// Shaders
static const int16_t sScreenWidth = 1280;
static const int16_t sScreenHeight = 720;


std::unique_ptr<IScene> mScene = std::make_unique<FirstTestScene>();

std::vector<Light> gLights;
static const uint16_t MAX_LIGHTS = 256;


SDL_Window* window;

std::unique_ptr<BloomEnvironment> mBloomEnvironment;


void InitializeOGL()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow("OpenGL", 100, 100, sScreenWidth, sScreenHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_MULTISAMPLE);

	SDL_GL_SetSwapInterval(0);
	// Define the viewport dimensions
	glViewport(0, 0, sScreenWidth, sScreenHeight);
}



void LoadModels(ModelLoader* const aModelLoader, GLTextureLoader* const aTextureLoader)
{
	aModelLoader->ClearProcessedMeshes();


	aModelLoader->LoadModel("Models\\Cube.obj");
	for (auto e : aModelLoader->GetMeshesToBeProcessed())
	{
		mBloomEnvironment->cubeMesh = std::make_shared<GLMesh>(e);
	}
	aModelLoader->ClearProcessedMeshes();
}


int main(int argc, char* argv[])
{
	
	InitializeOGL();

	std::unique_ptr<ModelLoader> modLoader = std::make_unique<ModelLoader>();
	std::unique_ptr<GLTextureLoader> texLoader = std::make_unique<GLTextureLoader>();
	std::shared_ptr<Input> input = std::make_shared<Input>();
	auto engTimer = std::make_unique<EngineTimer>();




	SDL_Event evt;
	
	
	mScene->Init();
	mScene->UploadModels(modLoader.get(), texLoader.get());
	mBloomEnvironment = std::make_unique<BloomEnvironment>();

	mBloomEnvironment->Initialize();
	LoadModels(modLoader.get(), texLoader.get());
	



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
		static bool shouldCompute = false;

		if (input->GetKeyDown(Primitive::Input::KEYS::SPACE))
		{
			usePhong = !usePhong;
		}

		if (input->GetKeyDown(Primitive::Input::KEYS::DOWNARROW))
		{
			shouldCompute = !shouldCompute;
		}
	
		mBloomEnvironment->Render(mScene.get());

		
	
		
		SDL_SetWindowTitle(window, std::to_string(1.0f / engTimer->GetDeltaTime()).c_str());
		SDL_GL_SwapWindow(window);
		
	}
	
	mBloomEnvironment->CleanUp();


	SDL_DestroyWindow(window);

	return 0;
}


