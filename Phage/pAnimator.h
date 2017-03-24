#pragma once
#include "pAnimation.h"

class pAnimator
{
public:
	pAnimator();
	~pAnimator();

	pSceneNode* _node;
	pAnimation* _animation;

	//CUrrent animation time
	double _currentTime;

	AnimationKeyframe _currentKeyframe;
	AnimationKeyframe _nextKeyframe;
};

