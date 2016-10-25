#include "pImage.h"
#include "pImageLoader.h"

pImage::pImage(std::string name, std::string imageDir)
{
	pImage(name, imageDir, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

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

void pImage::bindTexture()
{
	printf("Texture ID=%i", textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint pImage::getTextureID()
{
	return textureID;
}

std::string pImage::getName()
{
	return name;
}

GLuint pImage::getID()
{
	return ID;
}

void pImage::loadImage()
{
	pImageLoader loader = pImageLoader();

	//Load the image and store its width and height
	imageData = loader.loadImage((char*)imageDirectory.c_str());
	width = loader.getWidth();
	height = loader.getHeight();

	//Generate a textureID
	glGenTextures(1, &textureID);

	//Bind the new texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	//Feed the image data to our new texture (image is BGR format because of freeimage)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);

	//Generate image mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_2D, 0);
}
