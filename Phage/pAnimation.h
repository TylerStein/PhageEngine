#pragma once
#include "pSceneNode.h"

#define BONES_PER_VERTEX 4
struct VertexBoneData {

	unsigned int IDs[BONES_PER_VERTEX];
	float weights[BONES_PER_VERTEX];
};

class pAnimation
{
public:
	
	pAnimation();
	~pAnimation();

	bool _isSkeletal;

	pSceneNode* _animRoot;
};

