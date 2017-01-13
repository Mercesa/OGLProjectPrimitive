#include "GLTextureLoader.h"

#include <GL/glew.h>
#include <SOIL.h>

#include "AssetsDatabase.h"

using namespace Primitive;


GLTextureLoader::GLTextureLoader()
{
}


GLTextureLoader::~GLTextureLoader()
{
}
#include <iostream>
uint32_t GLTextureLoader::LoadTexture(const std::string& aString)
{
	GLuint textures[1];
	glGenTextures(1, textures);

	int32_t textureID = Primitive::GetTexture(aString);
	if (textureID == -1)
	{
		int width, height;
		unsigned char* image;

		image = SOIL_load_image(aString.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);  //Generate num_mipmaps number of mipmaps here.
		SOIL_free_image_data(image);
		//free(image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Primitive::AddToMap(aString, textures[0]);
		return textures[0];
	}
	else
	{
		std::cout << "Re-using asset" << std::endl;
		return textureID;
	}

}
