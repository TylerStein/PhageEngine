#pragma once
#include "pResourceManager.h"
#include "pAnimation.h"

typedef pResourceHandle<pAnimationClip> animationHandle;
class pAnimationManager
{
public:
	static pAnimationManager* instance() {
		if (_instance == nullptr) {
			_instance = new pAnimationManager();
		}
		
		return _instance;
	}

	pAnimationClip* getAnimation(std::string name);
	pAnimationClip* getAnimation(unsigned int animationID);
	pAnimationClip* getAnimation(animationHandle animHandle);

	animationHandle addAnimation(pAnimationClip* animationClip);
	bool removeAnimation(unsigned int animID);

private:
	pAnimationManager();
	~pAnimationManager();

	static pAnimationManager* _instance;

	pResourceManager<pAnimationClip>* _animationManager;
};

