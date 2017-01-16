#include "IScene.h"

using namespace Primitive;

#include <assert.h>


IScene::IScene()
{
	mCamera = std::make_shared<Camera>();
}

const std::vector<Light>& IScene::GetAllLights()
{ 
	return mLights; 
}

const std::shared_ptr<Camera> IScene::GetCamera() 
{ 
	assert(mCamera != nullptr);
	return mCamera; 
}