#pragma once
#include "pAnimation.h"

class pSceneObject;

//Plays animations on an object
class pAnimator {
public:
	pAnimator(Animation* anim, pSceneObject* parentObject);

	void setAnimation(Animation* newAnim);

	double getScrub() const;

	//Checks if this animation is ready to be played
	bool isReady() const;

	void moveScrub(double seconds);
	void updateAnimation();
private:
	//The animation to be played
	Animation* _animation;

	//The scene object this animation is attached to
	pSceneObject* _sceneObject;

	//Animation playback position
	double _scrub;
};

