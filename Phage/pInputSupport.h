#pragma once
#include <stdio.h>
#include <string.h>
#include <glm\vec2.hpp>
#include <GLFW\glfw3.h>

//http://www.glfw.org/docs/latest/input_guide.html
//http://www.glfw.org/docs/latest/group__input.html#ga01d37b6c40133676b9cea60ca1d7c0cc
class pInputSupport
{
private:
	static pInputSupport* _instance;
	pInputSupport();
	~pInputSupport();
public:
	static pInputSupport* instance();

	GLdouble seconds; //keep track of timer stuff
	GLint kbState; //keep track of your key board state
	GLint mState; //keep track of your mouse state
	bool stickyKBKeys = false; //keep track of the state of the keyboards sticky keys
	bool stickyMKeys = false; //keep track of the state of your mousees sticky keys
	GLFWcursor* cursor; //the variable that is the cursor;
	
	//timer stuff start
	double getTime()
	{
		seconds = glfwGetTime();
		return seconds;
	}

	void setTime(GLfloat t)
	{
		glfwSetTime(t);
	}
	//timer stuff end

	//event process start
	void pollEvents() //this processes the events that have already been recieved
	{
		glfwPollEvents();
	}

	void waitEvents() //this processes the events that need to update the content of the window
	{
		glfwWaitEvents();
	}

	void waitSetTime(GLfloat t) //sets the time for glfwWaitEventsTimeout(float timer)
	{
		glfwWaitEventsTimeout(t);
	}

	void stopWaitTime() //if you have a wait event going and want it to stop post an empty event in the queue
	{
		glfwPostEmptyEvent();
	}
	//event process end
	
	//keyboard process start
	void setKeyCallbacks(GLFWwindow* w, GLFWkeyfun k) //to be notified when a physical key is pressed or released or when it repeats set a key callback
	{
		glfwSetKeyCallback(w, k);
	}

	static void key_callback(GLFWwindow* w, GLint key, GLint scancode, GLint action, GLint mods) //call to check what action to do
	{
		//example code for setting key inputs
		//if (key == GLFW_KEY_E && action == GLFW_PRESS)
		//if (key == GLFW_KEY_E && action == GLFW_RELEASE)
		//if (key == GLFW_KEY_E && action == GLFW_REPEAT)
			//activate_airship();

		//alternet example for checking states and using key inputs
		//kbState = glfwGetKey(w, key);
		//if (kbState == GLFW_PRESS)
		//if (kbState == GLFW_RELEASE)
		//if (kbState == GLFW_REPEAT)
			//activate_airship();
	}

	int getcbState(GLFWwindow* w, GLint key)
	{
		kbState = glfwGetKey(w, key);
		return kbState;
	}
	
	void ToggleStickyKBKeys(GLFWwindow* w) //sometimes key events wont register if your triggering many events sticky keys fixes this by disallowing you to press the same key untel the event has been resolved
	{
		if (stickyKBKeys == true) //if Skeys = true, set to false
		{
			glfwSetInputMode(w, GLFW_STICKY_KEYS, GLFW_FALSE);
			stickyKBKeys = false;
			
		}
		else if (stickyKBKeys == false) //if Skeys = false, set to true
		{
			glfwSetInputMode(w, GLFW_STICKY_KEYS, GLFW_TRUE);
			stickyKBKeys = true;
		}
	}
	//end of keyboard stuff

	//start of mouse stuff
	void setCursorPosCallback (GLFWwindow* w, GLFWcursorposfun cbfun) //call this to be notified when the cursor moves over the window
	{
		glfwSetCursorPosCallback(w, cbfun);
	}

    void cursor_position_callback(GLFWwindow* w, GLdouble xpos, GLdouble ypos) //call this to see the position of the cursor
	{
		glfwGetCursorPos(w, &xpos, &ypos);
	}

	void disableCursor(GLFWwindow* w) //turn off cursor
	{
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	
	void hideCursor(GLFWwindow* w) //make cursor invisiable
	{
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void normalCursor(GLFWwindow* w) //restore cursor to is default mode
	{
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void setCursor(GLFWwindow* w) //to activate what cursor is being used
	{
		glfwSetCursor(w, cursor);
	}
	
	void setNULLCursor(GLFWwindow* w) //when you dont want a cursor
	{
		glfwSetCursor(w, NULL);
	}
	/*
	GLFWcursor* customCursor(GLFWimage* i, GLint xhot, GLint yhot)
	{
		unsigned char pixels[16 * 16 * 4];
		memset(pixels, 0xff, sizeof(pixels));
		//GLFWimage i;
		i->width = 16;
		i->height = 16;
		i->pixels = pixels;
		GLFWcursor* cursor = glfwCreateCursor(&i, 0, 0);
		return cursor;

		//i could not get the i stuff working
	}
	*/

	void standardCursor()
	{
		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	}

	void setMouseButtonCallback(GLFWwindow* w, GLFWmousebuttonfun cbfun) //to be notifyed when a mouse button is pressed
	{
		glfwSetMouseButtonCallback(w, cbfun);
	}

	void mouse_button_callback(GLFWwindow* window, GLint button, GLint action, GLint mods)
	{
		//an example of setting and using mouse inputs
		//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
			//popup_menu();

		//alternet example for checking states and using mouse inputs
		//int mstate = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		//if (mstate == GLFW_PRESS)
		//if (mstate == GLFW_RELEASE)
			//upgrade_cow();
	}

	int getmState(GLFWwindow* w, GLint key)
	{
		mState = glfwGetKey(w, key);
		return mState;
	}

	void toggleStickyMKeys(GLFWwindow* w) //sometimes key events wont register if your triggering many events sticky keys fixes this by disallowing you to press the same key untel the event has been resolved
	{
		if (stickyMKeys == true) //if Skeys = true, set to false
		{
			glfwSetInputMode(w, GLFW_STICKY_KEYS, GLFW_FALSE);
			stickyMKeys = false;

		}
		else if (stickyMKeys == false) //if Skeys = false, set to true
		{
			glfwSetInputMode(w, GLFW_STICKY_KEYS, GLFW_TRUE);
			stickyMKeys = true;
		}
	}

	void setScrollCallback(GLFWwindow *w, GLFWscrollfun cbfun)
	{
		glfwSetScrollCallback(w, cbfun);
	}
	//end of mouse stuff

	//start of joystick stuff
	void setJoyStickCallback(GLFWjoystickfun cbfun)
	{
		glfwSetJoystickCallback(cbfun);
	}

	GLint isPresent()
	{
		GLint present = glfwJoystickPresent(GLFW_JOYSTICK_1);
		return present;
	}

	void setJoystickAxes()
	{
		int count;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	}

	void setJoystickButtons()
	{
		int count;
		const unsigned char* axes = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
	}

	void setJoyStickName()
	{
		const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
	}
};
