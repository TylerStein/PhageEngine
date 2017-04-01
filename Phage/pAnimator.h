#pragma once
#include "pAnimation.h"

//Plays animations on an object
class pAnimator {
public:

	void setAnimation(pAnimation* newAnim);
private:
	//The animation to be played
	pAnimation* _animation;

	//Loop the animation?
	bool _loop;

	//Animation playback position
	double _scrub;
};

