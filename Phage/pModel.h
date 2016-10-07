#pragma once
#include "GL\glew.h"
#include <string>
#include "pAsset.h"
#include "pMaterial.h"

class pModel : public pAsset
{
public:
	pModel(char* name, pMaterial* material, GLfloat* vertices, GLuint numVertices);
	~pModel();

	std::string getName() override;
	GLuint getID() override;

	std::string name;
	pType type;
	pMaterial* material;
	GLuint vertCount;
	GLfloat* vertices;
	
};

