#include "pMaterial.h"


std::string pMaterial::getName()
{
	return name;
}

GLuint pMaterial::getID()
{
	return ID;
}

pMaterial::pMaterial()
	:pAsset(pType::MATERIAL)
{

}


pMaterial::~pMaterial()
{
}
