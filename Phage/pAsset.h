#pragma once
#include "pModel.h"
enum pType
{
	MODEL, MATERIAL, IMAGE, AUDIO
};


class pAsset
{
public:
	pType type;

	pAsset(pType type);
	~pAsset();
};

