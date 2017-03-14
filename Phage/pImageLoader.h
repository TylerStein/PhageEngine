#pragma once
#include "GL\glew.h"

struct ImageData {
	ImageData(GLubyte* data, GLuint width, GLuint height) {
		_data = data;
		_width = width;
		_height = height;
	}

	GLubyte* _data;
	GLuint _width;
	GLuint _height;
};

//Image loader provides a simple loadImage function that returns a structure of needed image data
class pImageLoader
{
public:
	//Static image data loading function
	static ImageData loadImage(char* filePath);

private:
	pImageLoader() = delete;
	~pImageLoader() = delete;
};

