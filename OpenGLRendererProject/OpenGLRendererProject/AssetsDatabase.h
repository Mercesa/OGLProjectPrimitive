#pragma once


#include <map>
namespace Primitive{

static std::map<std::string, uint32_t> textureMap;

static int32_t GetTexture(std::string aFilePath)
{
	auto iter = textureMap.find(aFilePath);
	if (iter != textureMap.end())
	{
		return iter->second;
	}

	else
	{
		return -1;
	}
}

static void AddToMap(std::string aFilePath, uint32_t aID)
{
	textureMap.insert(std::pair<std::string, uint32_t>(aFilePath, aID));
}

class Shader;
static std::map<std::string, Shader*> mShaderMap;


static void AddToShaderMap(std::string aFilePath, Shader* aShader)
{
	mShaderMap.insert(std::pair<std::string, Shader*>(aFilePath, aShader));
}


};