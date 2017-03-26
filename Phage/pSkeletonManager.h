#pragma once
#include "pSkeleton.h"
#include "pResourceManager.h"

typedef pResourceHandle<Skeleton> skeletonHandle;

class pSkeletonManager
{
public:
	static pSkeletonManager* instance() {
		if (_instance == nullptr) {
			_instance = new pSkeletonManager();
		}

		return _instance;
	}

	Skeleton* getSkeleton(unsigned int skeletonID);
	Skeleton* getSkeleton(std::string name);
	Skeleton* getSkeleton(skeletonHandle skelHandle);

	skeletonHandle addSkeleton(Skeleton* skeleton);
	bool removeSkeleton(unsigned int animID);
private:
	pSkeletonManager();
	~pSkeletonManager();

	static pSkeletonManager* _instance;

	pResourceManager<Skeleton>* _skeletonManager;

};

