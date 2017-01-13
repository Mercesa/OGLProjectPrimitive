#pragma once

#include <vector>

#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

namespace Primitive{

struct Vertex
{
	Vertex(){}
	Vertex(glm::vec3 &position, glm::vec3 &nor, glm::vec2 &uv, glm::vec3 &tang, glm::vec3 &btang)
		: Position(position), Normal(nor), UV(uv), Tangent(tang), BTangent(btang){}
	
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec3 Tangent;
	glm::vec3 BTangent;
};

enum TextureTypes
{
	eTEXTURETYPE_DIFFUSE,
	eTEXTURETYPE_SPECULAR,
	eTEXTURETYPE_BUMP
};

struct TextureData
{
	std::string mTextureFilePath;
	TextureTypes mTextureType;
};

struct MaterialData
{
	std::vector<TextureData> mTextures;
};

// meshdata is meant to hold indices/vertices, could be used for OGL -but- generation of bounding boxes etc as well
struct MeshData
{
	std::vector<Vertex> verticeData;
	std::vector<uint32_t> indicesData;
	MaterialData materialData;
};

}