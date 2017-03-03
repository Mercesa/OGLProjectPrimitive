#pragma once
#include "IRenderEnvironment.h"

#include "GLMesh.h"
namespace Primitive
{
	class BloomEnvironment :
		public IRenderEnvironment
	{
	public:
		BloomEnvironment();
		~BloomEnvironment();

		virtual void Initialize();
		virtual void Render(IScene* aScene);
		virtual void CleanUp();


		std::shared_ptr<GLMesh> cubeMesh;
	};
};

