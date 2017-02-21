#pragma once

class IScene;

namespace Primitive{

class Renderer
{
public:
	Renderer();
	~Renderer();

	void RenderScene(IScene* const aScene);
};

}