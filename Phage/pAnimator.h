#pragma once
#include "pAnimation.h"

//Plays animations on an object
class pAnimator {
public:

	//Retreive an animation layer
	pAnimation* getAnimationLayer(int idx) const;

	//Add an animation layer
	void addAnimationLayer(pAnimation* anim);

	//Remove an animation layer
	void removeAnimationLayer(int idx);
private:
	//One layer per animation
	std::vector<pAnimation*> _layers;

	//Loop the animation?
	bool _loop;

	//Animation playback position
	double _scrub;
};

