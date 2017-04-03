#pragma once
#include "IRenderEnvironment.h"

namespace Primitive
{


class ComputeRaytracerEnvironment :
	public IRenderEnvironment
{
public:
	ComputeRaytracerEnvironment();
	~ComputeRaytracerEnvironment();

	virtual void Initialize();
	virtual void Render(IScene* aScene);
	virtual void CleanUp();
};

};