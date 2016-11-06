#pragma once
#include "pAsset.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <map>
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
		delete keyList;
		delete valueList;
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

class pShader : public pAsset {
public:
	//32 Possible Property Flags (int = 32bits)
	enum PropertyFlags {
		MaterialInfo		= 1 << 0,	//Takes material data(Diffuse, Ambient, Specular, Shininess)
		Texture_Diffuse		= 1 << 1,	//Takes a texture2d sampler for a diffuse base texture
		Texture_Specular	= 1 << 2,	//Takes a texture2d sampler for a specular map texture
		Texture_Bump		= 1 << 3,	//Takes a texture2d sampler for a bump map texture
		Light_Affected		= 1 << 4	//Takes a list of pLight objects to be affected by
	};

	//Represents attributes for functions/flags
	enum Attributes {
		//Material Info
		DiffuseColor		= 1 << 1, //Diffuse color multiplier
		AmbientColor		= 1 << 2, //Ambient color multiplier
		SpecularColor		= 1 << 3, //Specular color multiplier
		Shininess			= 1 << 4, //Specular shine multiplier

		//Textures
		DiffuseTexture		= 1 << 5, //Diffuse texture
		SpecularTexture		= 1 << 6, //Specular texture
		BumpTexture			= 1 << 7, //Bump texture

		//View Matrices
		Camera_View			= 1 << 8, //The camera view matrix
		Projection_View		= 1 << 9, //The projection matrix
		Model_View			= 1 << 10, //The model movement matrix

		//Light Properties
		Light_Position		= 1 << 11,
		Light_Color			= 1 << 12,
		Light_Power			= 1 << 13,
		Light_Attenuation	= 1 << 14,
		Light_Ambient		= 1 << 15
	};

	pShader(std::string name, GLint flags, std::string vertexShaderPath, std::string fragmentShaderPath);
	~pShader();

	//Set property information in the shader
	void setPropertyMat4(GLuint attributeID, glm::mat4 data);
	void setPropertyVec3(GLuint attributeID, glm::vec3 data);
	void setPropertyVec2(GLuint attributeID, glm::vec2 data);
	void setPropertyFloat(GLuint attributeID, GLfloat data);
	void setPropertyTextureID(GLuint attributeID, GLuint textureID);

	//Compile the shader
	void compileShader(std::string vertexShaderPath, std::string fragmentShaderPath);

	//ShaderID
	GLuint getShaderID();

	//Asset name
	std::string getName();

	GLint getFlags();

	//Get the ID of property locations
	GLuint getPropertyID(const Attributes const propertyAttribute);

private:
	//std::map<const pShader::Attributes, GLuint>* attributeMap;
	SimpleKeyValue<pShader::Attributes, GLuint>* attributeMap;

	//Shader flags
	GLint flags;

	GLuint shaderID;
	std::string name;
};