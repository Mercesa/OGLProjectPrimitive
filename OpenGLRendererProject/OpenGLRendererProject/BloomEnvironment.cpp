#include "BloomEnvironment.h"

using namespace Primitive;

#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <memory>

#include "ShaderProgram.h"
#include "RenderingHelperFunctions.h"
#include "GLTimer.h"
#include "GLMesh.h"
#include "Shader.h"
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

BloomEnvironment::BloomEnvironment()
{
}


BloomEnvironment::~BloomEnvironment()
{
}


// Shaders
static const int16_t sScreenWidth = 1280;
static const int16_t sScreenHeight = 720;

glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float)sScreenWidth / (float)sScreenHeight, 0.1f, 1000.0f);

GLuint hdrFBO;
GLuint colorBuffers[2];
GLuint rboDepth;

// Ping pong framebuffer for blurring
GLuint pingpongFBO[2];
GLuint pingpongColorbuffers[2];
GLuint computeTexID;



std::shared_ptr<ShaderProgram> mProgramLight;
std::shared_ptr<ShaderProgram> mProgram;
std::shared_ptr<ShaderProgram> mProgramHDR;
std::shared_ptr<ShaderProgram> mProgramGaussian;
std::shared_ptr<ShaderProgram> mProgramCompute;
std::shared_ptr<ShaderProgram> mProgramGBufferFill;
std::shared_ptr<ShaderProgram> mProgramSSAO;

bool usePhong = true;
bool shouldCompute = false;

void InitializeShadersAndPrograms()
{
	auto basicVertexShader = std::make_unique<Shader>();
	basicVertexShader->LoadShader("Shaders\\VertexShaderBasicPosition.vert", eSHADER_VERTEX);

	auto basicFragmentShader = std::make_unique<Shader>();
	basicFragmentShader->LoadShader("Shaders\\FragmentShaderBasicPosition.frag", eSHADER_FRAGMENT);

	mProgram = std::make_shared<ShaderProgram>();
	mProgram->AddShader(basicVertexShader.get());
	mProgram->AddShader(basicFragmentShader.get());
	mProgram->CompileProgram();

	auto lightVertexShader = std::make_unique<Shader>();
	lightVertexShader->LoadShader("Shaders\\VertexShaderLight.vert", eSHADER_VERTEX);

	auto lightFragmentShader = std::make_unique<Shader>();
	lightFragmentShader->LoadShader("Shaders\\FragmentShaderLight.frag", eSHADER_FRAGMENT);

	mProgramLight = std::make_shared<ShaderProgram>();
	mProgramLight->AddShader(lightVertexShader.get());
	mProgramLight->AddShader(lightFragmentShader.get());
	mProgramLight->CompileProgram();

	auto hdrPostProcShaderVertex = std::make_unique<Shader>();
	hdrPostProcShaderVertex->LoadShader("Shaders\\PostProcHDR.vert", eSHADER_VERTEX);

	auto hdrPostprocShaderFragment = std::make_unique<Shader>();
	hdrPostprocShaderFragment->LoadShader("Shaders\\PostProcHDR.frag", eSHADER_FRAGMENT);

	mProgramHDR = std::make_shared<ShaderProgram>();
	mProgramHDR->AddShader(hdrPostProcShaderVertex.get());
	mProgramHDR->AddShader(hdrPostprocShaderFragment.get());
	mProgramHDR->CompileProgram();


	auto gaussianVertexShader = std::make_shared<Shader>();
	gaussianVertexShader->LoadShader("Shaders\\GaussianBlur.vert", eSHADER_VERTEX);

	auto gaussianFragmentShader = std::make_shared<Shader>();
	gaussianFragmentShader->LoadShader("Shaders\\GuassianBlur.frag", eSHADER_FRAGMENT);

	mProgramGaussian = std::make_shared<ShaderProgram>();
	mProgramGaussian->AddShader(gaussianVertexShader.get());
	mProgramGaussian->AddShader(gaussianFragmentShader.get());
	mProgramGaussian->CompileProgram();

	auto testComputeShader = std::make_shared<Shader>();
	testComputeShader->LoadShader("Shaders\\InitialTestCompute.comp", eSHADER_COMPUTE);

	mProgramCompute = std::make_shared<ShaderProgram>();
	mProgramCompute->AddShader(testComputeShader.get());
	mProgramCompute->CompileProgram(true);


	auto gBufferViewPassVertex = std::make_shared<Shader>();
	gBufferViewPassVertex->LoadShader("Shaders\\GBufferViewPass.vert", eSHADER_VERTEX);

	auto gBufferViewPassFragment = std::make_shared<Shader>();
	gBufferViewPassFragment->LoadShader("Shaders\\GBufferViewPass.frag", eSHADER_FRAGMENT);

	mProgramGBufferFill = std::make_shared<ShaderProgram>();
	mProgramGBufferFill->AddShader(gBufferViewPassVertex.get());
	mProgramGBufferFill->AddShader(gBufferViewPassFragment.get());
	mProgramGBufferFill->CompileProgram();

	auto ssaoPassVertex = std::make_shared<Shader>();
	ssaoPassVertex->LoadShader("Shaders\\ShaderSSAO.vert", eSHADER_VERTEX);

	auto ssaoPassFragment = std::make_shared<Shader>();
	ssaoPassFragment->LoadShader("Shaders\\ShaderSSAo.frag", eSHADER_FRAGMENT);

	mProgramSSAO = std::make_shared<ShaderProgram>();
	mProgramSSAO->AddShader(ssaoPassVertex.get());
	mProgramSSAO->AddShader(ssaoPassFragment.get());
	mProgramSSAO->CompileProgram();
}

void BloomEnvironment::Initialize()
{
	InitializeShadersAndPrograms();
	// Set up floating point framebuffer to render scene to
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// - Create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
	glGenTextures(2, colorBuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sScreenWidth, sScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, sScreenWidth, sScreenHeight);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// - Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sScreenWidth, sScreenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, sScreenWidth, sScreenHeight);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sScreenWidth, sScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &computeTexID);
	glBindTexture(GL_TEXTURE_2D, computeTexID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, sScreenWidth, sScreenHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void BloomEnvironment::Render(IScene* aScene)
{
	// Render
	// Clear the colorbuffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// Draw our first triangle
	mProgram->UseProgram();
	BindLights(mProgram->GetProgram(), aScene);
	glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(aScene->GetCamera()->GetViewMatrix()));
	glUniform1i(glGetUniformLocation(mProgram->GetProgram(), "usePhong"), (int)usePhong);

	glUniform3f(glGetUniformLocation(mProgram->GetProgram(), "cameraPosition"), aScene->GetCamera()->camPosition.x, aScene->GetCamera()->camPosition.y, aScene->GetCamera()->camPosition.z);


	for (auto o : aScene->GetSceneObjects())
	{
		glUniformMatrix4fv(glGetUniformLocation(mProgram->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(o->mTransform));

		for (auto e : o->mMeshes)
		{
			glUniform1i(glGetUniformLocation(mProgram->GetProgram(), "diffuseTexture"), 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, e->mTexture->GetTextureID());

			glBindVertexArray(e->mVAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawElements(GL_TRIANGLES, e->mAmountOfIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
	



	RenderLights(cubeMesh.get(), mProgramLight->GetProgram(), aScene, Projection);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	GLTimer mComputeTimer;
	mComputeTimer.Start();

	// Gaussian blur
	GLboolean horizontal = true, first_iteration = true;
	GLuint amount = 6;

	if (!shouldCompute)
	{
		mProgramGaussian->UseProgram();
		for (GLuint i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glUniform1i(glGetUniformLocation(mProgramGaussian->GetProgram(), "horizontal"), horizontal);
			glUniform1i(glGetUniformLocation(mProgramGaussian->GetProgram(), "image"), 0);

			glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)

			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	else
	{
		mProgramCompute->UseProgram();

		glBindImageTexture(0, colorBuffers[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
		glBindImageTexture(1, pingpongColorbuffers[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
		glDispatchCompute(sScreenWidth / 8, sScreenHeight / 8, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glUseProgram(0);
	}

	mComputeTimer.Stop();

	printf("Time spent on the GPU: %f ms\n", mComputeTimer.GetElapsedTime());

	// Post processing stage
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mProgramHDR->UseProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[0]);
	RenderQuad();
	glUseProgram(0);




}

void BloomEnvironment::CleanUp()
{

	// Properly de-allocate all resources once they've outlived their purpose

	glDeleteTextures(_countof(pingpongColorbuffers), pingpongColorbuffers);
	glDeleteTextures(_countof(colorBuffers), colorBuffers);
	glDeleteTextures(1, &computeTexID);
	glDeleteFramebuffers(1, &hdrFBO);
	glDeleteFramebuffers(_countof(pingpongFBO), pingpongFBO);
}