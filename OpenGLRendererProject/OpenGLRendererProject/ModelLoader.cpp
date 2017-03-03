#include "ModelLoader.h"

#include <Importer.hpp>
#include <Exporter.hpp>
#include <postprocess.h>
#include <scene.h>


#include "MeshData.h"

using namespace Assimp;
using namespace Primitive;

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

std::string mDirectory;

void ModelLoader::ClearProcessedMeshes()
{
	this->mMeshesToBeProcessed.clear();
}

void ProcessVertices(aiMesh* const a_Mesh, std::vector<Vertex>& a_Vertices)
{
	// Loop through vertices
	for (unsigned int i = 0; i < a_Mesh->mNumVertices; i++)
	{
		aiVector3D &vertice = a_Mesh->mVertices[i];
		aiVector3D &tang = a_Mesh->mTangents[i];
		aiVector3D &bTang = a_Mesh->mBitangents[i];

		Vertex vert;

		// Set position
		vert.Position.x = vertice.x;
		vert.Position.y = vertice.y;
		vert.Position.z = vertice.z;

		if (a_Mesh->HasTangentsAndBitangents())
		{
			// Set Tang
			vert.Tangent.x = tang.x;
			vert.Tangent.y = tang.y;
			vert.Tangent.z = tang.z;

			// Set BiTang
			vert.BTangent.x = bTang.x;
			vert.BTangent.y = bTang.y;
			vert.BTangent.z = bTang.z;
		}

		// Set normals
		if (a_Mesh->HasNormals())
		{
			vert.Normal.x = a_Mesh->mNormals[i].x;
			vert.Normal.y = a_Mesh->mNormals[i].y;
			vert.Normal.z = a_Mesh->mNormals[i].z;
		}

		// Set tex coords
		if (a_Mesh->HasTextureCoords(0))
		{
			//glm::vec2 tex;
			vert.UV.x = a_Mesh->mTextureCoords[0][i].x;
			vert.UV.y = a_Mesh->mTextureCoords[0][i].y;
		}

		// Put it in vec
		a_Vertices.push_back(vert);
	}
}

// Process indices
void ProcessIndices(aiMesh* const a_Mesh, std::vector<uint32_t>& a_Indices)
{
	if (a_Mesh->HasFaces())
	{
		for (unsigned int i = 0; i < a_Mesh->mNumFaces; i++)
		{
			for (unsigned int j = 0; j < a_Mesh->mFaces[i].mNumIndices; j++)
			{
				a_Indices.push_back(a_Mesh->mFaces[i].mIndices[j]);
			}
		}
	}
}


const std::vector<MeshData>& ModelLoader::GetMeshesToBeProcessed()
{
	return mMeshesToBeProcessed;
}

std::string GetTextureLocation(aiMaterial* const a_Mat, aiTextureType a_Type)
{
	int count = 0;
	// Check the amount of textures of a specific type
	if ((count = a_Mat->GetTextureCount(a_Type)) <= 0)
	{
		return "";
	}

	// Get texture filename
	aiString str;
	a_Mat->GetTexture(a_Type, 0, &str);
	std::string filename = std::string(str.C_Str());

	// directory can be removed
	filename = mDirectory + '\\' + filename;

	return filename;
}


// Goes through the material
TextureData ProcessMaterial(aiMesh* a_Mesh, const aiScene* a_Scene)
{
	TextureData texDat;
	if (a_Mesh->mMaterialIndex > 0)
	{
		aiMaterial* material = a_Scene->mMaterials[a_Mesh->mMaterialIndex];

		texDat.mTextureFilePath = GetTextureLocation(material, aiTextureType_DIFFUSE);

		bool textureSuccess = false;
	}
	return texDat;
}


#include <iostream>
// Process mesh
void ModelLoader::ProcessMesh(aiMesh* const a_Mesh, const aiScene* const a_Scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// Process vertices & indices
	ProcessVertices(a_Mesh, vertices);
	ProcessIndices(a_Mesh, indices);


	TextureData textData = ProcessMaterial(a_Mesh, a_Scene);


	MeshData meshData;
	meshData.verticeData = vertices;
	meshData.indicesData = indices;
	meshData.materialData.mTextures.push_back(textData);


	this->mMeshesToBeProcessed.push_back(meshData);
}

// This function will be called recursively if there is more than 1 node in a scene
void ModelLoader::ProcessNode(aiNode* const a_Node, const aiScene* const a_Scene)
{
	// Process meshes in node
	for (unsigned int i = 0; i < a_Node->mNumMeshes; i++)
	{
		ProcessMesh(a_Scene->mMeshes[a_Node->mMeshes[i]], a_Scene);
		
	}

	// Process children of scene recursively
	for (unsigned int j = 0; j < a_Node->mNumChildren; j++)
	{
		ProcessNode((a_Node->mChildren[j]), a_Scene);
	}
}


void ModelLoader::LoadModel(const char* const aFilePath)
{
	Assimp::Importer importer;

	const aiScene* scene;

	scene = importer.ReadFile(aFilePath, aiProcess_GenNormals |  aiProcess_GenUVCoords | aiProcess_FlipUVs | aiProcessPreset_TargetRealtime_Fast);
	assert(scene != nullptr);

	if (!scene)
	{
		printf("%s", importer.GetErrorString());
	}

	// Set directory string and c_string
	std::string aFilePathStr(aFilePath);

	mDirectory = aFilePathStr.substr(0, aFilePathStr.find_last_of("\\"));
	
	ProcessNode(scene->mRootNode, scene);
	printf("%i", mMeshesToBeProcessed.size());
}