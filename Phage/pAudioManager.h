#pragma once

#include "pResourceManager.h"
#include "pSoundSystem.h"

class pAudioManager
{
public:

	typedef pResourceHandle<pSoundSystem> HandleType;

	//default constructor/destructor
	pAudioManager();
	~pAudioManager();

	//Creates a soundSYstem from provided info
	HandleType createSoundSystem(std::string soundSystemName, std::string audioFilePath, bool loop);

	//Adds a sound to the manager
	HandleType addSoundSystem(std::string soundSystemName, pSoundSystem* soundSystem);

	//Retreives a soundSystem from the manager
	pSoundSystem* getSoundSystem(HandleType &handle);
	pSoundSystem* getSoundSystem(std::string soundSystemName);

	//Delete a soundSystem from the manager
	void deleteSoundSystem(std::string soundSystemName);

	//Clear all soundSystems from the manager
	void clear();

private:

	pResourceManager<pSoundSystem> soundSystemResources;

};

