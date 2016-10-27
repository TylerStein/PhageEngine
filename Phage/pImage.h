#pragma once
#include "pAsset.h"
#include "GL\glew.h"

class pImage :
	public pAsset
{
public:
	//Constructor requires a directory to work with
	pImage(std::string name, std::string imageDir);
	//Constructor takes a directory, filter type, and wrap mode
	pImage(std::string name, std::string imageDir, GLuint minFilterType, GLuint magFilterType, GLuint wrapMode);
	//Basic constructor creates a fill-in checkerboard material
	pImage(std::string name);

	~pImage();

	void bindTexture();

	//Creates a 4 pixel (12 float) checkerboard texture and assigns it to imageData
	void generateCheckerboard();

	//Returns the textureID
	GLuint getTextureID();

	std::string getName() override;
	GLuint getID() override;

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
	GLuint ID;
};

