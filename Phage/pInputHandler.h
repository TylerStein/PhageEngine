#pragma once
#include <GLFW/glfw3.h>
#include "iostream"
class pInputHandler
{
private:
	static pInputHandler* _instance;
	pInputHandler();
	~pInputHandler();

public:
	static pInputHandler* instance();
	

	static void setErrorCallback(GLFWerrorfun err);

	static void setCurrentContext(GLFWwindow *w);

	static void setKeyCallback(GLFWwindow *w, GLFWkeyfun cbfun);
	static void setInputMode(GLFWwindow *w, int mode, int value);
	static void setCharModsCallback(GLFWwindow *w, GLFWcharmodsfun cbfun);
	static void setCursorPosCallback(GLFWwindow *w, GLFWcursorposfun cbfun);
	static void setMouseButtonCallback(GLFWwindow* w, GLFWmousebuttonfun cbfun);
	static void setScrollCallback(GLFWwindow* w, GLFWscrollfun cbfun);
	static void setCursorEnterCallback(GLFWwindow* w, GLFWcursorenterfun cbfun);
	static void getCursorPos(GLFWwindow* w, double* x, double* y);
	static int getJoystickPresent(int joy);
	static const char* getJoystickName(int joy);
	static const float* getJoystickAxes(int joy, int* count);
	const unsigned char* getJoystickButtons(int joy, int* count);
	static double getTime(void);
	static const char* getClipboardString(GLFWwindow* w);
};

