#pragma once
#include "pAsset.h"

class pMaterial : public pAsset
{
public:
	pType type;
	std::string name;
	GLuint ID;

	std::string getName() override;
	GLuint getID() override;

	pMaterial();
	~pMaterial();
};

