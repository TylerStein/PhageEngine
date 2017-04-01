#include "pSkeletonManager.h"

pSkeletonManager* pSkeletonManager::_instance = 0;

Skeleton * pSkeletonManager::getSkeleton(unsigned int skeletonID)
{
	Skeleton* res = nullptr;

	skeletonHandle resHandler = skeletonHandle(skeletonID);
	res = _skeletonManager->get(resHandler);

	return res;
}

Skeleton * pSkeletonManager::getSkeleton(std::string name)
{
	Skeleton* res = nullptr;

	skeletonHandle resHandler = _skeletonManager->get(name);
	res = _skeletonManager->get(resHandler);

	return res;
}

skeletonHandle pSkeletonManager::addSkeleton(Skeleton * skeleton)
{
	return _skeletonManager->put(std::string(skeleton->getName()), skeleton);
}

bool pSkeletonManager::removeSkeleton(unsigned int skeletonID)
{
	skeletonHandle resHandler = skeletonHandle(skeletonID);

	try {
		std::string animName = _skeletonManager->get(resHandler)->getName();
		_skeletonManager->remove(animName);
	}
	catch (std::exception e) {
		return false;
	}

	return true;
}

Skeleton * pSkeletonManager::getSkeleton(skeletonHandle skelHandle)
{
	return _skeletonManager->get(skelHandle);
}

pSkeletonManager::pSkeletonManager()
{
	_skeletonManager = new pResourceManager<Skeleton>();
}


pSkeletonManager::~pSkeletonManager()
{
	_skeletonManager->clearKeysAndValues();
	_skeletonManager->~pResourceManager();
}
