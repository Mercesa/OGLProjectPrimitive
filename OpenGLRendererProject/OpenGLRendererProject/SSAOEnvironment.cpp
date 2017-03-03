#include "SSAOEnvironment.h"

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

SSAOEnvironment::SSAOEnvironment()
{
}


SSAOEnvironment::~SSAOEnvironment()
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

void SSAOEnvironment::Initialize()
{

	//mProgramGBufferFill->UseProgram();;
	//glUniform1i(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "gPosition"), 0);
	//glUniform1i(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "gNormal"), 1);
	//glUniform1i(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "gAlbedo"), 2); 
	//glUniform1i(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "ssao"), 3); 
	//mProgramSSAO->UseProgram();
	//glUniform1i(glGetUniformLocation(mProgramSSAO->GetProgram(), "gPosition"), 0);
	//glUniform1i(glGetUniformLocation(mProgramSSAO->GetProgram(), "gNormal"), 1);
	//glUniform1i(glGetUniformLocation(mProgramSSAO->GetProgram(), "texNoise"), 2);


	//mProgramHDR->UseProgram();
	//glUniform1i(glGetUniformLocation(mProgramHDR->GetProgram(), "hdrBuffer"), 0);
	//glUniform1i(glGetUniformLocation(mProgramHDR->GetProgram(), "brightnessBuffer"), 1);
	//
	//mProgramGaussian->UseProgram();
	//glUniform1i(glGetUniformLocation(mProgramGaussian->GetProgram(), "image"), 0);
	//
	//glUseProgram(0);
	//

	/* CREATE SSAO BFUFERS */

	//std::unique_ptr<GLFramebuffer> gBufferFB = std::make_unique<GLFramebuffer>(sScreenWidth, sScreenHeight);
	//
	//
	//GLuint ssaoFBO, ssaoBlurFBO;
	//glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	//GLuint ssaoColorBuffer, ssaoColorBufferBlur;
	//// - SSAO color buffer
	//glGenTextures(1, &ssaoColorBuffer);
	//glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sScreenWidth, sScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "SSAO Framebuffer not complete!" << std::endl;
	//
	//// - and blur stage
	//glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	//glGenTextures(1, &ssaoColorBufferBlur);
	//glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sScreenWidth, sScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	//std::default_random_engine generator;
	//std::vector<glm::vec3> ssaoKernel;
	//
	//for (GLuint i = 0; i < 64; ++i)
	//{
	//	glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
	//	sample = glm::normalize(sample);
	//	sample *= randomFloats(generator);
	//
	//	GLfloat scale = GLfloat(i) / 64.0;
	//
	//	scale = lerp(0.1f, 1.0f, scale * scale);
	//	sample *= scale;
	//	ssaoKernel.push_back(sample);
	//}
	//
	//std::vector<glm::vec3> ssaoNoise;
	//
	//for (GLuint i = 0; i < 16; i++)
	//{
	//	glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
	//	ssaoNoise.push_back(noise);
	//}
	//
	//GLuint noiseTexture; glGenTextures(1, &noiseTexture);
	//glBindTexture(GL_TEXTURE_2D, noiseTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//
	//glBindTexture(GL_TEXTURE_2D, 0);

}

void SSAOEnvironment::Render(IScene* aScene)
{
	//// Render
	//// Clear the colorbuffer
	////glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	////glBindFramebuffer(GL_FRAMEBUFFER, gBufferFB->gBuffer);
	////
	////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////
	////glCullFace(GL_BACK);
	////glEnable(GL_DEPTH_TEST);
	////
	////// Draw our first triangle
	////mProgramGBufferFill->UseProgram();
	////BindLights(mProgramGBufferFill->GetProgram(), mScene.get());
	////glUniformMatrix4fv(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	////glUniformMatrix4fv(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(mScene->GetCamera()->GetViewMatrix()));
	////glUniformMatrix4fv(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	////glUniform1i(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "usePhong"), (int)usePhong);
	////
	////glUniform3f(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "cameraPosition"), mScene->GetCamera()->camPosition.x, mScene->GetCamera()->camPosition.y, mScene->GetCamera()->camPosition.z);
	//
	//
	//for (auto e : mMeshesToBeRendered)
	//{
	//	//glUniform1i(glGetUniformLocation(mProgramGBufferFill->GetProgram(), "diffuseTexture"), 0);
	//
	//	//glActiveTexture(GL_TEXTURE0);
	//	//glBindTexture(GL_TEXTURE_2D, e->mTexture->GetTextureID());
	//
	//	glBindVertexArray(e->mVAO);
	//	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glDrawElements(GL_TRIANGLES, e->mAmountOfIndices, GL_UNSIGNED_INT, 0);
	//	glBindVertexArray(0);
	//}
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	////RenderLights(cubeMesh.get(), mProgramLight->GetProgram());
	////glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	////glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//
	//GLTimer mComputeTimer;
	//mComputeTimer.Start();
	//
	//// Gaussian blur
	////GLboolean horizontal = true, first_iteration = true;
	////GLuint amount = 6;
	////
	////if (!shouldCompute)
	////{
	////	mProgramGaussian->UseProgram();
	////	for (GLuint i = 0; i < amount; i++)
	////	{
	////		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
	////		glUniform1i(glGetUniformLocation(mProgramGaussian->GetProgram(), "horizontal"), horizontal);
	////		glUniform1i(glGetUniformLocation(mProgramGaussian->GetProgram(), "image"), 0);
	////	
	////		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
	////	
	////		RenderQuad();
	////		horizontal = !horizontal;
	////		if (first_iteration)
	////			first_iteration = false;
	////	}
	////	
	////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	////}
	////
	////else
	////{
	////	mProgramCompute->UseProgram();
	////	
	////	glBindImageTexture(0, colorBuffers[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
	////	glBindImageTexture(1, pingpongColorbuffers[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
	////	glDispatchCompute(sScreenWidth / 8, sScreenHeight / 8, 1);
	////
	////	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	////
	////	glUseProgram(0);
	////}
	//
	//mComputeTimer.Stop();
	//
	////printf("Time spent on the GPU: %f ms\n", mComputeTimer.GetElapsedTime());
	//
	////// Post processing stage
	////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////mProgramHDR->UseProgram();
	////glActiveTexture(GL_TEXTURE0);
	////glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	////glActiveTexture(GL_TEXTURE1);
	////glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[0]);
	////RenderQuad();
	////glUseProgram(0);
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	//glClear(GL_COLOR_BUFFER_BIT);
	//mProgramSSAO->UseProgram();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, gBufferFB->gPosition);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, gBufferFB->gNormal);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, noiseTexture);
	//// Send kernel + rotation 
	//for (GLuint i = 0; i < 64; ++i)
	//	glUniform3fv(glGetUniformLocation(mProgramSSAO->GetProgram(), ("samples[" + std::to_string(i) + "]").c_str()), 1, &ssaoKernel[i][0]);
	//glUniformMatrix4fv(glGetUniformLocation(mProgramSSAO->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	//RenderQuad();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);





}

void SSAOEnvironment::CleanUp()
{

	// Properly de-allocate all resources once they've outlived their purpose

	glDeleteTextures(_countof(pingpongColorbuffers), pingpongColorbuffers);
	glDeleteTextures(_countof(colorBuffers), colorBuffers);
	glDeleteTextures(1, &computeTexID);
	glDeleteFramebuffers(1, &hdrFBO);
	glDeleteFramebuffers(_countof(pingpongFBO), pingpongFBO);
}