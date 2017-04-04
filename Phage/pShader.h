#pragma once
#include "pAsset.h"
#include "pEnums.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <string>
#include "pHashtable.h"

//A simple two-vector class that works with a key-value system similar to a hashtable or map
template<class _key, class _value>
class SimpleKeyValue {
public:
	//Create a key and value list
	SimpleKeyValue(int sz = 0) {
		keyList = new std::vector<_key>(sz);
		valueList = new std::vector<_value>(sz);
	}

	~SimpleKeyValue() {
		//delete keyList;
		//delete valueList;
		clear();
		keyList = NULL;
		valueList = NULL;
	}

	//Clear key and value lists
	void clear() {
		keyList->clear();
		valueList->clear();
	}

	//Insert a key-value pair, returns the inserted _value
	_value insert(_key Key, _value Value) {
		keyList->push_back(Key);
		valueList->push_back(Value);

		return Value;
	}

	//Iterate through key list until match is found and return value list value at that location, -1 = NULL
	_value findValue(_key Key) {
		for (int i(0); i < keyList->size(); ++i) {
			if (keyList->at(i) == Key) {
				return valueList->at(i);
			}
		}

		return -1;
	}

	//Find a value by numeric location
	_value findValue(int location) {
		return valueList->at(location);
	}

	//Find a key by numeric location
	_key findKey(int location) {
		return keyList->at(location);
	}

	//Remove a key-value set by key
	void remove(_key Key) {
		//Create key and value vector iterators
		std::vector<_key>::iterator		keyIter;
		std::vector<_value>::iterator	valIter;

		//Set the value iterator to the value list start
		valIter = valueList->begin();

		//Iterate through the key list until key is found
		for (keyIter = keyList->begin(); keyIter != keyList->end(); ++keyIter) {
			++valIter;

			//If this iterator location points to the desired key to be removed
			if (*keyIter == key) {
				//Remove the values at the iterator location
				keyList->erase(keyIter);
				valueList->erase(valIter);
				return;
			}
		}

#ifdef _DEBUG
		std::cout << "Could not find pair to remove!" << std::endl;
#endif
	}

	//Get the size of the key list
	int Count() {
		return keyList->size();
	}

private:
	//Vector holds keys
	std::vector<_key>*		keyList;
	//Vector holds values
	std::vector<_value>*	valueList;
};

typedef SimpleKeyValue<Attributes, GLuint> attribLocMap;
typedef SimpleKeyValue<Uniforms, GLuint> uniformLocMap;

typedef SimpleKeyValue<Attributes, std::string> attribNameMap;
typedef SimpleKeyValue<Uniforms, std::string> uniformNameMap;

class pShader : public pAsset {
public:
	pShader(std::string name, attribNameMap attributeNames, uniformNameMap uniformNames, std::string vertexShaderPath, std::string fragmentShaderPath);
	~pShader();

	//Set property information in the shader
	void setUniformMat4(GLuint attributeID, glm::mat4 data);
	void setUniformVec3(GLuint attributeID, glm::vec3 data);
	void setUniformVec2(GLuint attributeID, glm::vec2 data);
	void setUniformFloat(GLuint attributeID, GLfloat data);
	void setUniformInt(GLuint attributeID, GLint data);
	void setUniformTextureID(GLuint attributeID, GLuint textureID);

	void setUniformVec3(GLuint attributeID, std::vector<glm::vec3> data);
	void setUniformFloat(GLuint attributeID, std::vector<GLfloat> data);
	void setUniformInt(GLuint attributeID, std::vector<GLint> data);

	//Check attribute/uniform existance (fast check via bitwise flags)
	bool hasAttribute(Attributes attrib);
	bool hasUniform(Uniforms uni);

	//Get variable ID from shader
	GLuint getAttributeLocation(std::string shaderVar);
	GLuint getUniformLocation(std::string shaderVar);

	//Search for the shader variable location and save it to the appropriate map
	bool addAttribute(Attributes targetAttrib, std::string shaderVar);
	bool addUniform(Uniforms targetUniform, std::string shaderVar);

	//Get IDs
	GLuint getAttributeID(Attributes attrib);
	GLuint getUniformID(Uniforms uni);

	//Compile the shader
	void compileShader();

	//ShaderID
	GLuint getShaderID();

	//Asset name
	std::string getName();

	//Shader flags
	GLint getUniformFlags();
	GLint getAttributeFlags();
private:
	//Maps of attributes/uniforms and their shader IDs
	attribLocMap* attributeMap;
	uniformLocMap* uniformMap;

	//Shader flags
	GLint uniformFlags;
	GLint attributeFlags;

	std::string fragPath;
	std::string vertPath;

	GLuint shaderID;
	std::string name;
};