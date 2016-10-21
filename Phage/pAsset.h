#pragma once
#include "gl/glew.h"
#include <string>
#include "pEnums.h"

class pAsset
{
public:
	/*
	pType type;
	GLuint ID;
	std::string name;*/

	virtual std::string getName() = 0;
	virtual GLuint getID() = 0;
};

