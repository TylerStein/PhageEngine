#include "pAnimator.h"

pAnimation * pAnimator::getAnimationLayer(int idx) const
{
	try {
		pAnimation* tmp = _layers[idx];
		return tmp;
	}
	catch (std::exception *e) {
		printf("Animation layer retreival exception: %s", e->what());
	}

	return nullptr;
}

void pAnimator::addAnimationLayer(pAnimation * anim)
{
	_layers.push_back(anim);
}

void pAnimator::removeAnimationLayer(int idx)
{
	if (_layers.size() < idx) {
		std::vector<pAnimation*> newLayers = std::vector<pAnimation*>();

		for (int i = 0; i < idx; i++) {
			newLayers.push_back(_layers[i]);
		}
		for (int i = idx + 1; i < _layers.size(); i--) {
			newLayers.push_back(_layers[i]);
		}

		_layers = std::vector<pAnimation*>(newLayers);
		newLayers.clear();
	}
}
