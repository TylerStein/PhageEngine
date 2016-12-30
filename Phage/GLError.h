#pragma once
#include <iostream>
#include "LogManager.h"
#include "GL/glew.h"

#define $PRINT_GLERROR GLError::printError(__FILE__, __LINE__)

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

		glewGetErrorString(glError);


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

		if (glError != GL_NO_ERROR) {
			std::cout << "OpenGL error @ (" << std::string(file) << "), (" << (int)line << ") : \n" << errorString << std::endl;
			returnCode = 1;
		}
		return returnCode;
	}

	static inline int checkShaderError(GLuint shaderID, std::string shaderName) {
		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			GLchar* infoLog = new GLchar[maxLength + 1];

			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

			std::cerr << "Shader Linking Failed in: " << shaderName << std::endl << infoLog << std::endl;

			return 1;
		}

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

	static inline void printActiveAttributes(GLuint programID) {
		GLint i;
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 32; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);
		printf("Active Attributes: %d\n", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveAttrib(programID, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
		}

		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
		printf("Active Uniforms: %d\n", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveUniform(programID, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		}

		printf("-----------------------------\n");
	}
};
