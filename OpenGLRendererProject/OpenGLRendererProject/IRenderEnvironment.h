#pragma once


namespace Primitive
{


class IScene;

class IRenderEnvironment
{
public:
	IRenderEnvironment();
	~IRenderEnvironment();

	virtual void Initialize() = 0;
	virtual void Render(IScene* aScene) = 0;
	virtual void CleanUp() = 0;
};
}