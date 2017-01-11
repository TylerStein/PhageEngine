#include "pAudioManager.h"



pAudioManager::pAudioManager()
{
}


pAudioManager::~pAudioManager()
{
	clear();
}

pAudioManager::HandleType pAudioManager::createSoundSystem(std::string soundSystemName, std::string audioFilePath, bool loop)
{
	pAudioManager::HandleType result(-1);

	pSoundSystem* sndSystm = new pSoundSystem(soundSystemName, audioFilePath, loop);
	result = soundSystemResources.put(soundSystemName, sndSystm);
	return result;
}

pAudioManager::HandleType pAudioManager::addSoundSystem(std::string soundSystemName, pSoundSystem* soundSystem)
{
	return soundSystemResources.put(soundSystemName, soundSystem);
}

pSoundSystem * pAudioManager::getSoundSystem(HandleType & handle)
{
	return soundSystemResources.get(handle);
}

pSoundSystem * pAudioManager::getSoundSystem(std::string soundSystemName)
{
	HandleType ref = soundSystemResources.get(soundSystemName);
	return soundSystemResources.get(ref);
}

void pAudioManager::deleteSoundSystem(std::string soundSystemName)
{
	soundSystemResources.remove(soundSystemName);
}

void pAudioManager::clear()
{
	soundSystemResources.clearKeysAndValues();
}


