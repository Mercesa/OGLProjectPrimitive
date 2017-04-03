#include "ComputeRaytracerEnvironment.h"


#include <GL\glew.h>
#include "stdafx.h"


#include "Shader.h"
#include "ShaderProgram.h"
#include "Globals.h"

using namespace Primitive;



ComputeRaytracerEnvironment::ComputeRaytracerEnvironment()
{
}


ComputeRaytracerEnvironment::~ComputeRaytracerEnvironment()
{
}	

GLuint tex_output;




std::shared_ptr<ShaderProgram> mComputeRayProgram;
std::shared_ptr<ShaderProgram> mImagePlaneProgram;

static const int16_t sScreenWidth = 512;
static const int16_t sScreenHeight = 512;

void ComputeRaytracerEnvironment::Initialize()
{


	glGenTextures(1, &tex_output);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, sScreenWidth, sScreenHeight, 0, GL_RGBA, GL_FLOAT,
		NULL);

	int work_grp_cnt[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	printf("max global (total) work group size x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

	auto computeRaytraceShader = std::make_unique<Shader>();
	computeRaytraceShader->LoadShader("Shaders\\RaytracerCompute.comp", eSHADER_COMPUTE);

	mComputeRayProgram = std::make_shared<ShaderProgram>();
	mComputeRayProgram->AddShader(computeRaytraceShader.get());
	mComputeRayProgram->CompileProgram(true);


	auto postProcPlaneVertex = std::make_unique<Shader>();
	postProcPlaneVertex->LoadShader("Shaders\\ImagePlanePostProc.vert", eSHADER_VERTEX);

	auto postProcPlaneFragment = std::make_unique<Shader>();
	postProcPlaneFragment->LoadShader("Shaders\\ImagePlanePostProc.frag", eSHADER_FRAGMENT);

	mImagePlaneProgram = std::make_shared<ShaderProgram>();
	mImagePlaneProgram->AddShader(postProcPlaneVertex.get());
	mImagePlaneProgram->AddShader(postProcPlaneFragment.get());

	mImagePlaneProgram->CompileProgram();

}
// Taken from learnopengl.com, could've done it myself but too lazy right now

static inline void RenderQuad()
{
	static GLuint quadVAO = 0;
	static GLuint quadVBO;
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


void ComputeRaytracerEnvironment::Render(IScene* aScene)
{

	mComputeRayProgram->UseProgram();
	glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute((GLuint)sScreenWidth/16, (GLuint)sScreenHeight/16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


	glClear(GL_COLOR_BUFFER_BIT);
	mImagePlaneProgram->UseProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output);
	RenderQuad();
	glUseProgram(0);
}

void ComputeRaytracerEnvironment::CleanUp()
{
	glDeleteTextures(1, &tex_output);
}