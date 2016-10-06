#include "pModel.h"

pModel::pModel(char* name, pType type, pMaterial material, GLfloat* vertices, GLuint numVertices)
	:pAsset(type)
{
	this->name = name;
	this->type = type;
	this->material = material;
	
	this->vertices = new GLfloat[numVertices];
	for (int i(0); i < numVertices; ++i) {
		this->vertices[i] = vertices[i];
	}
}

pModel::~pModel()
{
	delete vertices;
}
