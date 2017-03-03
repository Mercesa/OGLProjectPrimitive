#pragma once
#include "IRenderEnvironment.h"

#include "GLMesh.h"

using namespace Primitive;

class SSAOEnvironment :
	public IRenderEnvironment
{
public:
	SSAOEnvironment();
	~SSAOEnvironment();

	virtual void Initialize();
	virtual void Render(IScene* aScene);
	virtual void CleanUp();


	std::shared_ptr<GLMesh> cubeMesh;

};


