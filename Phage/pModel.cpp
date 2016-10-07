#include "pModel.h"

pModel::pModel(char* name, pMaterial* material, GLfloat* vertices, GLuint numVertices)
	:pAsset(pType::MODEL)
{
	this->name = name;
	this->type = pType::MODEL;
	this->material = material;
	
	this->vertices = vertices;
}

pModel::~pModel()
{
	delete vertices;
}

std::string pModel::getName()
{
	return name;
}

GLuint pModel::getID()
{
	return ID;
}
