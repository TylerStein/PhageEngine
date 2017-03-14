#pragma once
#include "gl/glew.h"
#include <string>
#include "pEnums.h"

class pAsset
{
public:
	virtual std::string getName() const {
		return name;
	}

protected:
	std::string name;
};

