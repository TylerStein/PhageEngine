#pragma once
#include <iostream>
#include "LogManager.h"
#include "GL/glew.h"

class GLError {
public:
	static inline int logError(char* file, int line) {
		GLenum glError;
		int returnCode = 0;

		glError = glGetError();
		std::string errorString = "";
		switch (glError) {
		case GL_INVALID_ENUM:
			errorString = "A function has been called with an inappropriate enum";
			break;
		case GL_INVALID_VALUE:
			errorString = "A function has been called with an out of range numeric value";
			break;
		case GL_INVALID_OPERATION:
			errorString = "An operation has been performed out of context or not allowed in the current state";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorString = "The framebuffer object is not yet complete";
			break;
		case GL_OUT_OF_MEMORY:
			errorString = "System has run out of memory";
			break;
		default:
			errorString = "No error";
		}


		if (glError != GL_NO_ERROR) {
			LogManager::instance()->error("OpenGL error @ (" + (std::string)file + "), (" + (char*)line + "): \n" + errorString + "\n");
			returnCode = 1;
		}
		return returnCode;
	}

	static inline int printError(char* file, int line) {
		GLenum glError;
		int returnCode = 0;

		glError = glGetError();
		std::string errorString = "";
		switch (glError) {
		case GL_INVALID_ENUM:
			errorString = "A function has been called with an inappropriate enum";
			break;
		case GL_INVALID_VALUE:
			errorString = "A function has been called with an out of range numeric value";
			break;
		case GL_INVALID_OPERATION:
			errorString = "An operation has been performed out of context or not allowed in the current state";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorString = "The framebuffer object is not yet complete";
			break;
		case GL_OUT_OF_MEMORY:
			errorString = "System has run out of memory";
			break;
		default:
			errorString = "No error";
		}

		glError = glGetError();
		if (glError != GL_NO_ERROR) {
			std::cout << ("OpenGL error @ (" + (std::string)file + "), (" + (char*)line + "): \n" + errorString) << std::endl;
			returnCode = 1;
		}
		return returnCode;
	}

	static inline int checkLinkError(GLuint programID) {
		int res = 0;

		GLint linked;
		glGetProgramiv(programID, GL_LINK_STATUS, &linked);
		if (!linked) {
			res = 1;
			GLsizei len;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(programID, len, &len, log);
			std::cerr << "Shader linking failed: " << log << std::endl;
			delete[] log;
		}
		return res;
	}
};