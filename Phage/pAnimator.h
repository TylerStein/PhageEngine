#pragma once
#include "pAnimation.h"

//An animator to be attached to a scene object
class pAnimator
{
public:
	pAnimator();
	~pAnimator();
	

	double _scrub; //Location of current playback
	pAnimationClip* _animation; //The actual animation to apply
};

