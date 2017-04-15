//#include "pAnimator.h"
#include "pSceneObject.h"

pAnimator::pAnimator(Animation * anim, pSceneObject* parentObject)
{
	_animation = anim;
	_sceneObject = parentObject;
	_playbackRate = 1.0f;
}

void pAnimator::setAnimation(Animation * newAnim)
{
	_animation = newAnim;
}

Animation * pAnimator::getAnimation() const
{
	return _animation;
}

double pAnimator::getScrub() const
{
	return _scrub;
}

bool pAnimator::isReady() const
{
	//Is an animation present?
	if (_animation == nullptr) return false;
	//Is this attached to a scene object?
	if (_sceneObject == nullptr) return false;
	//Does the associated scene object have a model?
	return _sceneObject->hasModel();
}

void pAnimator::setScrub(double seconds)
{
	_scrub = seconds;
	cropScrub();
}

void pAnimator::moveScrub(double seconds)
{
	_scrub += _playbackRate * seconds * _animation->GetFrameRate();
	cropScrub();
}

void pAnimator::updateAnimation()
{
	_animation->Update(_scrub);
}

void pAnimator::setPlaybackRate(float rate)
{
	_playbackRate = rate;
}

void pAnimator::cropScrub()
{
	if (_scrub > _animation->GetDuration()) {
		_scrub = _animation->GetDuration();
	}
	else if (_scrub < 0) { _scrub = 0; }
}
