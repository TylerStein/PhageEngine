#include "pImage.h"
#include "pImageLoader.h"
#include "DefaultPaths.h"

#include <iostream>
#include "GLError.h"

pImage::pImage(std::string name, std::string imageDir, GLuint minFilterType, GLuint magFilterType, GLuint wrapMode)
{
	this->name = name;
	imageDirectory = imageDir;
	this->minFilterType = minFilterType;
	this->magFilterType = magFilterType;
	this->wrapMode = wrapMode;

	loadImage();

}

pImage::~pImage()
{

}

GLuint pImage::getWidth() const
{
	return width;
}

GLuint pImage::getHeight() const
{
	return height;
}

void pImage::generateCheckerboard()
{
	const GLuint pixelCount = 4;
	const GLuint unitCount = pixelCount * 3;

	//Create some checkerboard texture data
	GLfloat textureData[unitCount] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	//Allocate memory for image data
	imageData = (GLubyte*)malloc(unitCount * sizeof(GLfloat));
	imageData = (GLubyte*)memcpy(imageData, textureData, sizeof(textureData));

	/*
	//Copy over the texture data
	for (int i(0); i < unitCount; ++i) {
		imageData[i] = textureData[i];
	}*/

	width = 2;
	height = 2;
	minFilterType = GL_NEAREST;
	magFilterType = GL_NEAREST;
	wrapMode = GL_CLAMP_TO_EDGE;

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}


GLuint pImage::getTextureID()
{
	return textureID;
}

void pImage::loadImage()
{
	if (imageDirectory == "") {
		//generateCheckerboard();
		imageDirectory = image_white;
		loadImage();
	}
	else {
		//Load the image and store its width and height
		ImageData tmpData = pImageLoader::loadImage((char*)imageDirectory.c_str());

		imageData = tmpData._data;
		width = tmpData._width;
		height = tmpData._height;
	}
}

void pImage::setupTexture(GLenum texturePlace)
{
	//Generate a textureID
	glGenTextures(1, &textureID);
	

	//Set the active texture to 0
	glActiveTexture(texturePlace);
	//GLError::printError(__FILE__, __LINE__);

	//Bind the new texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	

	//Feed the image data to our new texture (image is BGR format because of freeimage)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)imageData);

	//Make a border color (black)
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	//Generate image mipmaps if using mipmaps
	if ((magFilterType == GL_LINEAR_MIPMAP_LINEAR || magFilterType == GL_LINEAR_MIPMAP_NEAREST)
		|| (minFilterType == GL_LINEAR_MIPMAP_LINEAR || minFilterType == GL_LINEAR_MIPMAP_NEAREST))
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}



	
}

void pImage::useTexture(GLenum texturePlace)
{
	//Set the active texture to 0
	glActiveTexture(texturePlace);

	//Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void pImage::unuseTexture(GLenum texturePlace)
{
	//Set the active texture to 0
	glActiveTexture(texturePlace);

	//Bind no texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(0);
}
