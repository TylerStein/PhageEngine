#include "pLightManager.h"


pLightManager* pLightManager::_instance = 0;

pLightManager * pLightManager::instance()
{
	if (_instance == 0) {
		_instance = new pLightManager();
	}
	return _instance;
}

std::vector<pLight*> pLightManager::getAllLights()
{
	return *lightsInScene;
}

pLightManager::pLightManager()
{
	lightsInScene = new std::vector<pLight*>();
}


pLightManager::~pLightManager()
{
}

void pLightManager::clearLights()
{
	lightsInScene->clear();
	lightsInScene->shrink_to_fit();
}

pLight* pLightManager::addLight(pLight * light)
{
	lightsInScene->push_back(light);
	return light;
}
