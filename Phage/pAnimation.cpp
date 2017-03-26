#include "pAnimation.h"

pAnimationClip::pAnimationClip(std::string name, double duration, double frameRate, bool loop)
{
	this->name = name;
	_framesPerSecond = frameRate;
	_duration = duration;
	_isLooping = loop;
}

bool pAnimationClip::isLooping() const
{
	return _isLooping;
}

double pAnimationClip::duration() const
{
	return _duration;
}

double pAnimationClip::framesPerSecond() const
{
	return _framesPerSecond;
}


void pAnimationClip::setLooping(bool toLoop)
{
	_isLooping = toLoop;
}

void pAnimationClip::setPlaybackRate(double newFramesPerSecond)
{
	_framesPerSecond = newFramesPerSecond;
}
