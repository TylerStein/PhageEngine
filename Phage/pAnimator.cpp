//#include "pAnimator.h"
#include "pSceneObject.h"

pAnimator::pAnimator(Animation * anim, pSceneObject* parentObject)
{
	_animation = anim;
	_sceneObject = parentObject;
}

void pAnimator::setAnimation(Animation * newAnim)
{
	_animation = newAnim;
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

void pAnimator::moveScrub(double seconds)
{
	_scrub += seconds * _animation->GetFrameRate();
	if (_scrub > _animation->GetDuration()) {
		_scrub = _animation->GetDuration();
	}
	else if (_scrub < 0) { _scrub = 0; }
}

void pAnimator::updateAnimation()
{
	
}
