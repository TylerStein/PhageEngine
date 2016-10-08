#include "pAsset.h"


pAsset::pAsset(pType type) {
	this->type = type;
}

pAsset::~pAsset() {

}

std::string pAsset::getName()
{
	return name;
}

GLuint pAsset::getID()
{
	return ID;
}

