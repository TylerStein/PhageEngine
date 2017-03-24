#pragma once
#include "pSceneNode.h"

struct AnimationKeyframe {
	glm::vec3 _position;
	glm::quat _rotation;
	double _timestep;
};

class pAnimation
{
public:
	
	pAnimation();
	~pAnimation();

	bool _isSkeletal;

	double _duration;
	double _ticksPerSecond;

	std::string _name;

	pSceneNode* _animRoot;

	AnimationKeyframe* _keyframes[];
};

