#pragma once
#include "pAsset.h"
#include "GL\glew.h"

class pImage :
	public pAsset
{
public:
	//Constructor has defaults: takes a name, directory, filter type, and wrap mode
	pImage(std::string name = "NONE", std::string imageDir = "", GLuint minFilterType = GL_LINEAR, GLuint magFilterType = GL_LINEAR, GLuint wrapMode = GL_REPEAT);

	~pImage();

	void bindTexture();

	GLuint getWidth() const;
	GLuint getHeight() const;

	//Creates a 4 pixel (12 float) checkerboard texture and assigns it to imageData
	void generateCheckerboard();

	//Returns the textureID
	GLuint getTextureID();

	std::string getName() override;

private:
	//Uses pImageLoader to get the image data
	void loadImage();
	
	//ID of the openGL texture object
	GLuint textureID;
	//Data containing the image
	char* imageData;
	//Directory of the image file
	std::string imageDirectory;
	//Mini filter type
	GLuint minFilterType;
	//Magnified filter type
	GLuint magFilterType;
	//Wrap mode enum
	GLuint wrapMode;
	//Image width and height
	GLuint width, height;

	std::string name;
};

