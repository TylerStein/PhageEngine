#pragma once
#include "GL\glew.h"

class pImageLoader
{
public:
	pImageLoader();
	~pImageLoader();

	char* loadImage(char* filePath);

	GLuint getWidth();
	GLuint getHeight();

private:
	char* filePath;
	char* imageData;
	GLuint width;
	GLuint height;
};

