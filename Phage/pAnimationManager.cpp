#include "pAnimationManager.h"

pAnimationManager* pAnimationManager::_instance = 0;

Animation * pAnimationManager::getAnimation(std::string name)
{
	Animation* res = nullptr;

	animationHandle resHandler = _animationManager->get(name);
	res = _animationManager->get(resHandler);

	return res;
}

Animation * pAnimationManager::getAnimation(unsigned int animationID)
{
	Animation* res = nullptr;

	animationHandle resHandler = animationHandle(animationID);
	res = _animationManager->get(resHandler);

	return res;
}

Animation * pAnimationManager::getAnimation(animationHandle animHandle)
{
	Animation* res = nullptr;

	res = _animationManager->get(animHandle);

	return res;
}

animationHandle pAnimationManager::addAnimation(Animation * animation)
{
	return _animationManager->put(animation->getName(), animation);
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
	_animationManager = new pResourceManager<Animation>();
}


pAnimationManager::~pAnimationManager()
{
	_animationManager->clearKeysAndValues();
	_animationManager->~pResourceManager();
}

