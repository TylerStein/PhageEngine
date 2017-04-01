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

void pAnimation::addAnimationClip(pAnimationClip * anim)
{
	_animationClips.push_back(anim);
}

void pAnimation::removeAnimationClip(std::string name)
{
	std::vector<pAnimationClip*> newList = std::vector<pAnimationClip*>();
	for (int i = 0; i < _animationClips.size(); i++) {
		if (_animationClips[i]->getName() != name) {
			newList.push_back(_animationClips[i]);
		}
	}
	_animationClips = std::vector<pAnimationClip*>(newList);
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
	this->name = name;
}

pAnimationClip::~pAnimationClip()
{
}

pBasicAnimationClip::pBasicAnimationClip(std::string name) 
	: pAnimationClip(name)
{
}

pSkeletalAnimationClip::pSkeletalAnimationClip(std::string name) : pAnimationClip(name)
{
}
