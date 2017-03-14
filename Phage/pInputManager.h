#pragma once
#include <GLFW/glfw3.h>
#include "iostream"
#include <vector>
#include "pEnums.h"
#include "pInputHandler.h"

//here is a key
struct keyStruct
{
	keyName name; //whats the name of this key
	double pressTime; //how long has this been pressed for
	bool currentlyDown; //is this pressed or realeaced
	bool keyHeld; //has this been pressed for atleast two blips
	bool pThisFrame; //has this keys press been responded to

	//default constructor
	keyStruct()
	{
		name = KEY_UNKNOWN;
		pressTime = 0;
		currentlyDown = false;
		keyHeld = false;
		pThisFrame = false;
	}

	//advanced consructor
	keyStruct(double lastPressed, bool UpOrDown, bool boolHeld, bool pressedThisFrame, keyName kname)
	{
		pressTime = lastPressed; //how long has this been pressed for
		currentlyDown = UpOrDown; //is this pressed or realeaced
		keyHeld = boolHeld; //has this been pressed for atleast two blips
		name = kname; //whats the name of this key
		pThisFrame = pressedThisFrame; //has this keys press been responded to
	}
};

class pInputManager
{
private:
	//singleton accessor + private con/destructor
	static pInputManager* _instance;
	pInputManager();
	~pInputManager();

public:
	//objects used to store or access information
	static pInputManager* instance();
	std::vector<keyStruct> keyRegistry;

	void Initialize(GLFWwindow* window, pInputHandler* inputHandler);

	//keyboard methods
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void controllKey_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void characterMod_callback(GLFWwindow *window, unsigned int key, int mods);
	static void key_logger(int ke, int scancod, int act, int mod);

	//keyboard methods that are advanced / todo with the keyRegistry
	static void advancedKey_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	void keyStruct_Register(double pressTime, bool keyLastPressed, bool iChecked, keyName sName, pInputManager* pInstance, int state);
	void inputUpdate(double deltaTime);
	bool hasKeyBeenChecked(keyName name);
	bool hasKeyBeenRepeated(keyName name);
	bool hasKeyBeenPressed(keyName name);

	//mouse methods
	static void cursorPositionCallback(GLFWwindow *window, double xPosition, double yPosition);
	static void cursorEnterCallback(GLFWwindow *window, int enteredScreen);
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
	static void creCursor(GLFWwindow *window, GLFWimage img, int xHot, int yHot);
	static void posLogger(double xPosition, double yPosition);

	//controller methods
	static void presentCheck(int present);
	static void axesTest(const float *axes);
	static void axesControlls(const float *axes);
	static void buttonControlls(const unsigned char *buttons);
	static void controllerName(const char *name, int num);

	//time methods
	static void timeSenceInit(double time);
	static void modTime(double time, double mod);

	//Clipboard method
	static void paste(const char *text);
	static void setText(char txt[], GLFWwindow *window);
};

