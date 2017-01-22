#include "FirstTestScene.h"

using namespace Primitive;

#include <time.h>
FirstTestScene::FirstTestScene()
{
}


FirstTestScene::~FirstTestScene()
{
}

void FirstTestScene::Init()
{
	srand(time(NULL));

	//for (int i = 0; i < 256; i++)
	//{	//
	//	Light l;
	//	float randomX = ((rand() % 140) + -80)/10.0f;
	//	float randomY = (rand() % 110)/10.0f;
	//	float randomZ = ((rand() % 160) + -100.0f)/10.0f;
	//	
	//	float randomColX = (rand() % 75 + 25)/100.0f;
	//	float randomColY = (rand() % 75 + 25)/100.0f;
	//	float randomColZ = (rand() % 75 + 25)/100.0f;
	//	
	//	
	//	//std::cout << randomX << std::endl;
	//	
	//	l.position = glm::vec3(randomX, randomY, randomZ);
	//	l.diffuseColor = glm::vec3(randomColX, randomColY, randomColZ);
	//	l.specularColor = glm::vec3(randomColX, randomColY, randomColZ);
	//	mLights.push_back(l);
	//}	

	Light l;
	l.position = glm::vec3(0.0f, 2.0f, 1.0f);
	l.diffuseColor = glm::vec3(5.0f, 0.0f, 5.0f);
	l.specularColor = glm::vec3(5.0f, 0.0f, 5.0f);
	mLights.push_back(l);
}


void FirstTestScene::Update(float dt, std::shared_ptr<Input> aInput)
{
	if (aInput->GetKey(Input::KEYS::A))
	{
		mCamera->Strafe(dt, cameraMovement::CAM_LEFT);
	}

	if (aInput->GetKey(Input::KEYS::W))
	{
		mCamera->Strafe(dt, cameraMovement::CAM_UP);
	}

	if (aInput->GetKey(Input::KEYS::S))
	{
		mCamera->Strafe(dt, cameraMovement::CAM_DOWN);
	}

	if (aInput->GetKey(Input::KEYS::D))
	{
		mCamera->Strafe(dt, cameraMovement::CAM_RIGHT);
	}
}


void FirstTestScene::OnMouseMove(std::shared_ptr<Input> aInput)
{
	this->mCamera->MoveWithMouse(aInput->mouseRelativeMoved.x, aInput->mouseRelativeMoved.y);
}



