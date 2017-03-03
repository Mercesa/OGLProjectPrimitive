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

	void Initialize();
	void Render(IScene* aScene);
	void CleanUp();
};

};