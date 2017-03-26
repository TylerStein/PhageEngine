#include "pAnimationManager.h"

pAnimationManager* pAnimationManager::_instance = 0;

pAnimationClip * pAnimationManager::getAnimation(std::string name)
{
	pAnimationClip* res = nullptr;

	animationHandle resHandler = _animationManager->get(name);
	res = _animationManager->get(resHandler);

	return res;
}

pAnimationClip * pAnimationManager::getAnimation(unsigned int animationID)
{
	pAnimationClip* res = nullptr;

	animationHandle resHandler = animationHandle(animationID);
	res = _animationManager->get(resHandler);

	return res;
}

pAnimationClip * pAnimationManager::getAnimation(animationHandle animHandle)
{
	pAnimationClip* res = nullptr;

	res = _animationManager->get(animHandle);

	return res;
}

animationHandle pAnimationManager::addAnimation(pAnimationClip * animationClip)
{
	return _animationManager->put(animationClip->getName(), animationClip);
}

bool pAnimationManager::removeAnimation(unsigned int animID)
{

	animationHandle resHandler = animationHandle(animID);

	try {
		std::string animName = _animationManager->get(resHandler)->getName();
		_animationManager->remove(animName);
	}
	catch (std::exception e) {
		return false;
	}

	return true;
}

pAnimationManager::pAnimationManager()
{
	_animationManager = new pResourceManager<pAnimationClip>();
}


pAnimationManager::~pAnimationManager()
{
	_animationManager->clearKeysAndValues();
	_animationManager->~pResourceManager();
}

