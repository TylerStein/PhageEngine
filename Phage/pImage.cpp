#include "pImage.h"
#include "pImageLoader.h"

#include <iostream>

pImage::pImage(std::string name, std::string imageDir, GLuint minFilterType, GLuint magFilterType, GLuint wrapMode)
{
	this->name = name;
	imageDirectory = imageDir;
	this->minFilterType = minFilterType;
	this->magFilterType = magFilterType;
	this->wrapMode = wrapMode;
	if (imageDir != "") {
		loadImage();
	}
	else {
		generateCheckerboard();
	}
}

pImage::~pImage()
{

}

void pImage::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
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
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f
	};

	//Allocate memory for image data
	imageData = (char*)malloc(unitCount * sizeof(GLfloat));

	//Copy over the texture data
	for (int i(0); i < unitCount; ++i) {
		imageData[i] = textureData[i];
	}

	width = 2;
	height = 2;
	minFilterType = GL_LINEAR;
	magFilterType = GL_LINEAR;
	wrapMode = GL_REPEAT;
}

GLuint pImage::getTextureID()
{
	return textureID;
}

std::string pImage::getName()
{
	return name;
}

void pImage::loadImage()
{
	if (imageDirectory != "") {
		pImageLoader loader = pImageLoader();

		//Load the image and store its width and height
		imageData = loader.loadImage((char*)imageDirectory.c_str());
		width = loader.getWidth();
		height = loader.getHeight();

		//Generate a textureID
		glGenTextures(1, &textureID);

		//Bind the new texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Feed the image data to our new texture (image is BGR format because of freeimage)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_FLOAT, (void*)imageData);

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
}
