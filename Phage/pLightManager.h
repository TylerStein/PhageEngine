#pragma once
#include "pLight.h"
#include <vector>

class pLightManager
{
public:
	static pLightManager* instance();

	std::vector<pLight*> getAllLights();
	void clearLights();
	pLight* addLight(pLight* light);
private:
	pLightManager();
	~pLightManager();



	std::vector<pLight*>* lightsInScene;

	static pLightManager* _instance;
};

