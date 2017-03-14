#pragma once
#include "pAsset.h"
#include "GL\glew.h"

class pImage :
	public pAsset
{
public:
	friend class pMaterial;

	//Constructor has defaults: takes a name, directory, filter type, and wrap mode
	pImage(std::string name = "NONE", std::string imageDir = "", GLuint minFilterType = GL_LINEAR, GLuint magFilterType = GL_LINEAR, GLuint wrapMode = GL_REPEAT);

	~pImage();

	GLuint getWidth() const;
	GLuint getHeight() const;

	//Creates a 4 pixel (12 float) checkerboard texture and assigns it to imageData
	void generateCheckerboard();

	//Returns the textureID
	GLuint getTextureID();

private:
	//Uses pImageLoader to get the image data
	void loadImage();

	void setupTexture(GLenum texturePlace);
	void useTexture(GLenum texturePlace);
	void unuseTexture(GLenum texturePlace);
	
	//ID of the openGL texture object
	GLuint textureID;
	//Data containing the image
	GLubyte* imageData;
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
};

