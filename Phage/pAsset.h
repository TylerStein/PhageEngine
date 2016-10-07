#pragma once
#include "gl/glew.h"
#include <string>
#include "pEnums.h"

class pAsset
{
public:
	pType type;
	GLuint ID;
	std::string name;

	pAsset(pType type);
	~pAsset();

	virtual std::string getName();
	virtual GLuint getID();
};

