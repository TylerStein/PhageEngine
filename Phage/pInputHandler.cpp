#include "pInputHandler.h"

//singleton
pInputHandler* pInputHandler::_instance = 0;
pInputHandler* pInputHandler::instance()
{
//Create a new instance if none exists
	if (_instance == 0)
	{
		_instance = new pInputHandler();
	}
	//else return private instance veriable
	return _instance;
}

//constructor
pInputHandler::pInputHandler()
{

}

//destructor
pInputHandler::~pInputHandler()
{
	//glfwDestroyCursor(cursor);
	delete _instance;
}

//wrapper for setting a glfw error callback
void pInputHandler::setErrorCallback(GLFWerrorfun err)
{
	glfwSetErrorCallback(err);
}

//wrapper for setting a glfw window context
void pInputHandler::setCurrentContext(GLFWwindow * w)
{
	glfwMakeContextCurrent(w);
}

//wrapper for setting the glfw keycallback (the method that gets triggered when keys are pressed)
void pInputHandler::setKeyCallback(GLFWwindow * w, GLFWkeyfun cbfun)
{
	glfwSetKeyCallback(w, cbfun);
}

//wrapper for setting the input mode for GLFW (cursors, mouse buttons, keys) sticky keys, mouse settings ect
void pInputHandler::setInputMode(GLFWwindow * w, int mode, int value)
{
	glfwSetInputMode(w, mode, value);
}

//like key callback but to be used for text, symbols and characters rather than complex controlls
void pInputHandler::setCharModsCallback(GLFWwindow * w, GLFWcharmodsfun cbfun)
{
	glfwSetCharModsCallback(w, cbfun);
}

//wrapper for GLFW mouse callback, this calls a method when the mouse is moved about
void pInputHandler::setCursorPosCallback(GLFWwindow * w, GLFWcursorposfun cbfun)
{
	glfwSetCursorPosCallback(w, cbfun);
}

//wrapper for GLFW mouse click callback, this calls a method when the mouse is clicked, or clacked
void pInputHandler::setMouseButtonCallback(GLFWwindow * w, GLFWmousebuttonfun cbfun)
{
	glfwSetMouseButtonCallback(w, cbfun);
}

//wrapper for GLFW wheel callback, calling a function when scrolling has happned
void pInputHandler::setScrollCallback(GLFWwindow * w, GLFWscrollfun cbfun)
{
	glfwSetScrollCallback(w, cbfun);
}

//wrapper for GLFW that calls a method when the cursor enters / leaves the window
void pInputHandler::setCursorEnterCallback(GLFWwindow * w, GLFWcursorenterfun cbfun)
{
	glfwSetCursorEnterCallback(w, cbfun);
}

//wrapper for getting the cursor position in the form of two doubles, this is called when the mouse moves
void pInputHandler::getCursorPos(GLFWwindow * w, double * x, double * y)
{
	glfwGetCursorPos(w, x, y);
}

//a wrapper for GLFW to test if a joystick is enabled
int pInputHandler::getJoystickPresent(int joy)
{
	int pres = glfwJoystickPresent(joy);
	return pres;
}

//a wrapper for GLFW to get and return a joysticks name
const char* pInputHandler::getJoystickName(int joy)
{
	const char* name = glfwGetJoystickName(joy);
	return name;
}

//a wrapper for GLFW to count and return the number of controller axes available
const float * pInputHandler::getJoystickAxes(int joy, int * count)
{
	const float *controllerAxis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, count);
	return controllerAxis;
}

//a wrapper for GLFW to count and return the number of controller buttons available
const unsigned char * pInputHandler::getJoystickButtons(int joy, int * count)
{
	const unsigned char *cButtons = glfwGetJoystickButtons(joy, count);
	return cButtons;
}

//a wrapper for GLFW to count and return glfw's time function 
double pInputHandler::getTime(void)
{
	double seconds = glfwGetTime();
	return seconds;
}

//a wrapper for GLFW to return what was coppied to the users tray
const char * pInputHandler::getClipboardString(GLFWwindow * w)
{
	const char *clipText = glfwGetClipboardString(w);
	return clipText;
}
