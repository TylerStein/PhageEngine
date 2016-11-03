#pragma once
#include "GL\glew.h"

class pImageLoader
{
public:
	pImageLoader();
	~pImageLoader();

	GLubyte* loadImage(char* filePath);

	GLuint getWidth();
	GLuint getHeight();

private:
	GLubyte* imageData;
	char* filePath;
	GLuint width;
	GLuint height;
};

