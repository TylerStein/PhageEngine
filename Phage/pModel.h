#pragma once
#include "pAsset.h"
#include "pMaterial.h"
#include "GL\glew.h"
#include <string>

class pModel:public pAsset
{
public:
	pModel(char* name, pType type, pMaterial material, GLfloat* vertices, GLuint numVertices);
	~pModel();
	std::string name;
	pType type = pType::MODEL;
	pMaterial material;
	GLuint vertCount;
	GLfloat* vertices;
	
};

