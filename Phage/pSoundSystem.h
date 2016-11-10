#pragma once

#include <iostream>
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_common.h"
#include "FMOD/fmod_errors.h"

class pSoundSystem
{
private:
	//the system that the sound will run off
	FMOD::System *system;
	//result variable that will be used to check for errors
	FMOD_RESULT result;
	//used to check the current version of FMOD
	unsigned int version;
	//Sound object that will represent the audio that will be played
	FMOD::Sound *sound;
	//Channel object that will represent the channel this sound will play through
	FMOD::Channel *channel;
	//used to quit
	bool quitFlag;
	//used to track whether sound is currently playing on this soundObject
	bool isPlaying = false;
	//set the name in storage
	std::string name;

public:
	//used to assign file path for desired sound for this object
	std::string filePath;
	//controls whether you want to loop the sound or not
	bool doLoop;

	//default constructor
	pSoundSystem(void);

	//constructor that takes the file path to set
	//filePth: the file path to the audio file
	//loop: boolean value, true if you want audio to loop, false if not
	pSoundSystem(std::string nm, std::string filePth, bool loop);
	~pSoundSystem(void);

	//initializes the sound system
	void Init();

	//method that supports error checks. each check sees if FMOD_OK is returned when checked.
	//if that is the case, continue, if not, throw error and report what caused it
	void FMODErrorCheck(FMOD_RESULT result);

	//plays the given sound
	void playSound();

	//pauses the sound if already playing
	void pauseSound();

	//resumes the sound if paused
	void resumeSound();

	//set the filepath for the desired sound file
	void setFilePath(std::string filePth)
	{
		filePath = filePth;
	}

	//sets whether you want sound to loop or not
	void setLoop(bool choice)
	{
		doLoop = choice;
	}

	std::string getName()
	{
		return name;
	}

	//update that is to be called every frame. FMOD supplied update method that does regular checks on the system
	void update();

};

