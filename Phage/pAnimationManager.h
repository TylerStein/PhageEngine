#pragma once
#include "pResourceManager.h"
#include "pAnimation.h"

typedef pResourceHandle<Animation> animationHandle;
class pAnimationManager
{
public:
	static pAnimationManager* instance() {
		if (_instance == nullptr) {
			_instance = new pAnimationManager();
		}
		
		return _instance;
	}

	Animation* getAnimation(std::string name);
	Animation* getAnimation(unsigned int animationID);
	Animation* getAnimation(animationHandle animHandle);

	animationHandle addAnimation(Animation* animation);
	bool removeAnimation(unsigned int animID);

private:
	pAnimationManager();
	~pAnimationManager();

	static pAnimationManager* _instance;

	pResourceManager<Animation>* _animationManager;
};

