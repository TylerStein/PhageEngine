#include "pSoundSystem.h"

pSoundSystem::pSoundSystem(void)
{
}

pSoundSystem::pSoundSystem(std::string nm, std::string filePth, bool loop)
{
	name = nm;
	setFilePath(filePth);
	setLoop(loop);
	Init();
}

pSoundSystem::~pSoundSystem(void)
{
	FMODErrorCheck(sound->release());
	FMODErrorCheck(system->release());
}

void pSoundSystem::Init()
{
	//create FMOD inteface object
	result = FMOD::System_Create(&system);
	FMODErrorCheck(result);

	//check the version
	result = system->getVersion(&version);
	FMODErrorCheck(result);

	if (version < FMOD_VERSION)
	{
		std::cout << "Error! You are using an old version of FMOD " << version << ". This program requires " << FMOD_VERSION << std::endl;
		exit(0);
	}

	//Initialize FMOD
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	FMODErrorCheck(result);

	//open sound as a stream
	if (doLoop)
	{
		result = system->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, 0, &sound);
	}
	else
	{
		result = system->createSound(filePath.c_str(), FMOD_DEFAULT, 0, &sound);
	}
	FMODErrorCheck(result);

	//Sounds should repeat forever (Maybe)
	if (doLoop)
	{
		channel->setLoopCount(-1);
	}
	else
	{
		channel->setLoopCount(0);
	}
}

void pSoundSystem::FMODErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD ERROR: (" << result << ") - " << FMOD_ErrorString(result) << std::endl;
	}
}

void pSoundSystem::playSound()
{
	system->playSound(sound, 0, false, 0);
	isPlaying = true;
}

void pSoundSystem::pauseSound()
{
	channel->setPaused(true);
	isPlaying = false;
}

void pSoundSystem::resumeSound()
{
	channel->setPaused(false);
	isPlaying = true;
}

void pSoundSystem::update()
{
	system->update();
}
