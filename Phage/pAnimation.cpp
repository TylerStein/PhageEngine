#include "pAnimation.h"

pAnimation::pAnimation(std::string name, double duration, double frameRate, bool loop)
{
	this->name = name;
	_framesPerSecond = frameRate;
	_duration = duration;
	_isLooping = loop;
}

bool pAnimation::isLooping() const
{
	return _isLooping;
}

double pAnimation::duration() const
{
	return _duration;
}

double pAnimation::framesPerSecond() const
{
	return _framesPerSecond;
}


void pAnimation::setLooping(bool toLoop)
{
	_isLooping = toLoop;
}

void pAnimation::setPlaybackRate(double newFramesPerSecond)
{
	_framesPerSecond = newFramesPerSecond;
}

void pAnimationClip::setStartTime(float time)
{
	if (time >= 0.0f && time < _endTime) {
		_startTime = time;
	}
}

float pAnimationClip::getStartTime() const
{
	return _startTime;
}

void pAnimationClip::setEndTime(float time)
{
	if (time > _startTime) {
		_endTime = time;
	}
}

float pAnimationClip::getEndTime() const
{
	return _endTime;
}

pVertexAnimationClip::pVertexAnimationClip(std::string name) 
	: pAnimationClip(name)
{
}

pAnimationClip::pAnimationClip(std::string name)
{
}

pAnimationClip::~pAnimationClip()
{
}
