#include "pInputManager.h"
#include <stdio.h>

//singleton
	pInputManager* pInputManager::_instance = 0;
	pInputManager* pInputManager::instance()
	{
		//Create a new instance if none exists
		if (_instance == 0)
		{
			_instance = new pInputManager();
		}
		return _instance;
	}

	void pInputManager::Initialize(GLFWwindow* window, pInputHandler * inputHandler)
	{

	}

	//constructor
	pInputManager::pInputManager()
	{
		keyRegistry = std::vector<keyStruct>();
	}

	//destructor
	pInputManager::~pInputManager()
	{
		//glfwDestroyCursor(cursor);
		delete _instance;
	}

	//set error callback
	void pInputManager::error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description); //prints the error with description
	}

	//keyboard methods begin here
	//consider this the keyboard controlls its blank / basic and only returns the time pressed
	void pInputManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		double totalTime = glfwGetTime();
		std::cout << "key_callback was pressed at = " << totalTime << std::endl;
	}

	//this method is like the one above but less bare
	//example of how controlls are created and tested using GLFW ENUMS
	void pInputManager::controllKey_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		//std::cout << key << std::endl;
		//std::cout << action << std::endl;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		} //end of if esc 

		else if (key == GLFW_KEY_R)
		{
			switch (action)
			{
			case GLFW_PRESS:
				std::cout << "the 'R' key has been pressed" << std::endl;
				break;

			case GLFW_REPEAT:
				std::cout << "the 'R' key is being held down" << std::endl;
				break;

			case GLFW_RELEASE:
				std::cout << "the 'R' key has been released" << std::endl;
				break;
			}//end of switch
		}//end of else if R

		//key_logger(key, scancode, action, mods);
	}
	//end of key callback
	
	//to be used with text input rather than key input
	void pInputManager::characterMod_callback(GLFWwindow *window, unsigned int key, int mods)
	{
		std::cout << key << " : " << mods << std::endl;
	}
	
	//this was just a thing made to make sure the GLFW stuff was working
	void pInputManager::key_logger(int ke, int scancod, int act, int mod)
	{
		std::cout << "key pressed = " << ke << std::endl;
		std::cout << "scan code = " << scancod << std::endl;
		std::cout << "action = " << act << std::endl; //1 = pressed, 2 = held and 0 = released
		std::cout << "mods = " << mod << std::endl;
	}

	//here is the method that uses the KeyStructure
	void pInputManager::advancedKey_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		//track time
		double time = glfwGetTime();
		//create instance due to it being a static method
		pInputManager* tempContainer = pInputManager::instance();
		//call the key registering function
		tempContainer->keyStruct_Register(time, true, true, static_cast<keyName>(key), tempContainer, action);
	}

	//this method is called by advancedKey_callback it is not static
	void pInputManager::keyStruct_Register(double pressTime, bool keyLastPressed, bool iChecked, keyName sName, pInputManager* pInstance, int state)
	{
		keyStruct fKey; //create a empty keyStructure
		//loop through the registered keys
		for (int x = 0; x < keyRegistry.size(); ++x)
		{
			//check if the keys at x == the key value
			if (keyRegistry.at(x).name == static_cast<keyName>(sName))
			{
				//check key state and update the values of the key pressed
				if (state == GLFW_PRESS)
				{
					std::cout << static_cast<keyName>(sName) <<  " IS PRESSED" << std::endl;
					keyRegistry.at(x).currentlyDown = true; //if im pressed i must be down
					keyRegistry.at(x).pThisFrame = true; //if i got pressed i must be delt with
				}
				//else if (state == GLFW_REPEAT) //not required
				//{
				//	std::cout << static_cast<keyName>(sName) << " IS HELD" << std::endl;
				//	keyRegistry.at(x).currentlyDown = true;
				//	keyRegistry.at(x).keyHeld = true;

				//}
				else if (state == GLFW_RELEASE)
				{
					std::cout << static_cast<keyName>(sName) <<  " IS RELEASED" << std::endl;
					keyRegistry.at(x).currentlyDown = false;//if im released i must be up
					keyRegistry.at(x).keyHeld = false;//no way im held if im not touched
					keyRegistry.at(x).pThisFrame = false;//im not down i dont need to be delt with
					keyRegistry.at(x).pressTime = 0;//reset to 0
					
				}
				//if this if executes then return because were dont want to create a new key
				return;
			}
		}
		//register a key if one is pressed but not registered
		if (fKey.name == KEY_UNKNOWN)
		{
			//register the keys if its pressed but not registered
			if (state == GLFW_PRESS)
			{
				fKey = keyStruct(0, true, false, true, static_cast<keyName>(sName));//create real key
				std::cout << "IM Created as PRESSED" << std::endl;
				pInstance->keyRegistry.push_back(fKey);//push key to keyRegistry
			}
			else if (state == GLFW_RELEASE)
			{
				fKey = keyStruct(0, false, false, false, static_cast<keyName>(sName));//create real key
				std::cout << "IM Created as RELEASED" << std::endl;
				pInstance->keyRegistry.push_back(fKey);//push key to keyRegistry
			}
			
		}
	}

	//here is the update method for the InputManager
	void pInputManager::inputUpdate(double deltaTime)
	{
		//loop through the register of keys
		for (int i = 0; i < keyRegistry.size(); i++)
		{
			keyStruct tempKey = keyRegistry.at(i); //accessor not the actual object

			//keyName kn = keyRegistry.at(i).name;
			//bool pressedState = hasKeyBeenPressed(kn);
			//std::cout << keyRegistry.at(i).name << " Has the 'up or down' value set to : " << pressedState << std::endl;

			//if a looped key is found to be down, update their time held and call the hasKeyBeenChecked method
			if (tempKey.currentlyDown == true)
			{
				keyName kn = keyRegistry.at(i).name;
				bool checkedState = hasKeyBeenChecked(kn); //even if your not checking the bool returned, we still want this active so it can set a value within the register
				//std::cout << keyRegistry.at(i).name << " Has the 'checked this frame' value set to : " << checkedState << std::endl;
				//bool heldState = hasKeyBeenRepeated(kn);
				//std::cout << keyRegistry.at(i).name << " Has the 'is held' value set to : " << heldState << std::endl;
				keyRegistry.at(i).pressTime += deltaTime; //trying to set it from the registry does work
				//tempKey.pressTime += deltaTime; //trying to set it from the accessor does not work
				std::cout << keyRegistry.at(i).name << " Has been pressed for : " << keyRegistry.at(i).pressTime << " blips!" << std::endl;
			}
		}
	}

	//test to see if a key has been delt with
	bool pInputManager::hasKeyBeenChecked(keyName name)
	{
		//loop through the register
		for (int i = 0; i < keyRegistry.size(); i++)
		{
			if (keyRegistry.at(i).name == name) //if the name is what we want
			{
				if (keyRegistry.at(i).pThisFrame == true) //if the key has not been delt with
				{
					keyRegistry.at(i).pThisFrame = false; //deal with key
					return true; //return true if key has been delt with
				}
			}
		}
		return false; //if no key has been delt with return false
	}

	//check to see if a key is held for more than one calculation
	bool pInputManager::hasKeyBeenRepeated(keyName name)
	{
		for (int i = 0; i < keyRegistry.size(); i++) //loop the registry
		{
			if (keyRegistry.at(i).name == name) //check for wanted key
			{
				if (keyRegistry.at(i).currentlyDown == true && keyRegistry.at(i).pThisFrame == false) //if its down and yet has not been managed
				{
					keyRegistry.at(i).keyHeld = true;//alter value
					return true;//if this ran then its clearly heald
				}
			}
		}
		return false;//if this did not run then its clearly not being held
	}

	//return true if a key is down and false if the key is up
	bool pInputManager::hasKeyBeenPressed(keyName name)
	{
		for (int i = 0; i < keyRegistry.size(); i++)//loop registry
		{
			if (keyRegistry.at(i).name == name) //check for keys name
			{
				if (keyRegistry.at(i).currentlyDown == true)
				{
					return true; //if the key is down return true
				}
				else
				{
					return false; //if they key is not down return false
				}
			}
		}
		return false;//if this never hit then return false
	}
	//keyboard methods end

	//mouse methods start
	//prints the users mouse position
	void pInputManager::cursorPositionCallback(GLFWwindow *window, double xPosition, double yPosition)
	{
		std::cout << xPosition << " : " << yPosition << std::endl;
	}

	//print a tell if the mouse enters or leaves the screen
	void pInputManager::cursorEnterCallback(GLFWwindow *window, int enteredScreen)
	{
		if (enteredScreen)
		{
			std::cout << "Within Window" << std::endl;
		}
		else
		{
			std::cout << "Out of Window" << std::endl;
		}
	}

	//a controll callback from when mouse buttons have been pressed
	void pInputManager::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			std::cout << "LMB pressed" << std::endl;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_REPEAT) {
			std::cout << "LMB held" << std::endl;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			std::cout << "LMB released" << std::endl;
		}
	}
	
	//a callback that alerts the user how much they are scrolling by
	void pInputManager::scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
	{
		std::cout << "Offsets = " << xOffset << " : " <<yOffset << std::endl;
	}

	//a custrom cursor function
	void pInputManager::creCursor(GLFWwindow *window, GLFWimage img, int xHot, int yHot)
	{
		GLFWcursor *cursor = glfwCreateCursor(&img, xHot, yHot);
		glfwSetCursor(window, cursor);
	}

	//a method that prints the position of the mouse only diffrence with this and cursorPositionCallback is that this does not take a window
	void pInputManager::posLogger(double xPosition, double yPosition)
	{
		std::cout << "posLogger = " << xPosition << " : " << yPosition << std::endl;
	}

	//returns 0 if no controllers plugged in and return a non 0 value if one or more is
	void pInputManager::presentCheck(int present)
	{
		std::cout << "Controller status = " << present << std::endl;
	}

	//this method shows us what controller axeses are being moved about
	void pInputManager::axesTest(const float * axes)
	{
		std::cout << "Left Stick X Axis" << axes[0] << std::endl;
		std::cout << "Left Stick Y Axis" << axes[1] << std::endl;
		std::cout << "Right Stick X Axis" << axes[2] << std::endl;
		std::cout << "Right Stick Y Axis" << axes[3] << std::endl;
		std::cout << "Left Trigger L2 Push" << axes[4] << std::endl;
		std::cout << "Right Trigger R2 Push" << axes[5] << std::endl;
		pInputManager::axesControlls(axes); //then call the controlls
	}

	//controll the controlleraxes this is an example
	void pInputManager::axesControlls(const float * axes)
	{
		if (axes[1] == 0)
		{
			std::cout << "Left Stick Y is idle at 0" << std::endl;
		}
		else
		{
			std::cout << "Left Stick Y Axis is moving about" << std::endl;
		}

		if (axes[0] == 0)
		{
			std::cout << "Left Stick X is idle at 0" << std::endl;
		}
		else
		{
			std::cout << "Left Stick X Axis is moving about" << std::endl;
		}

	}

	//controll the controllers buttons
	void pInputManager::buttonControlls(const unsigned char * buttons)
	{

		if (GLFW_PRESS == buttons[0])
		{
			std::cout << "0 button pressed" << std::endl;
		}
		
		if (GLFW_PRESS == buttons[1])
		{
			std::cout << "1 button pressed" << std::endl;
		}
		if (GLFW_PRESS == buttons[2])
		{
			std::cout << "2 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[3])
		{
			std::cout << "3 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[4])
		{
			std::cout << "4 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[5])
		{
			std::cout << "5 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[6])
		{
			std::cout << "6 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[7])
		{
			std::cout << "7 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[8])
		{
			std::cout << "8 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[9])
		{
			std::cout << "9 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[10])
		{
			std::cout << "10 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[11])
		{
			std::cout << "11 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[12])
		{
			std::cout << "12 button pressed" << std::endl;
		}

		if (GLFW_PRESS == buttons[13])
		{
			std::cout << "13 button pressed" << std::endl;
		}
	}

	//display a relavant name of the controller
	void pInputManager::controllerName(const char * name, int num)
	{
		std::cout << "Controller " << num << " is named : " << name << std::endl;
	}

	//total time elapsed
	void pInputManager::timeSenceInit(double time)
	{
		std::cout << time << std::endl;
	}

	//set add to the current time
	void pInputManager::modTime(double time, double mod)
	{
		double varTime = time + mod;
		glfwSetTime(varTime);
	}

	//print what is in your cliptray
	void pInputManager::paste(const char * text)
	{
		std::cout << text << std::endl;
	}

	//set the clip of the clipboardtray to a character array
	void pInputManager::setText(char txt[], GLFWwindow *window)
	{
		glfwSetClipboardString(window, txt);
	}
