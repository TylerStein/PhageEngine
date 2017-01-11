#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <string>
#include "pModel.h"

class pModelLoader
{
public:
	/*Function returns an instance of the
	  pModelLoader object. Creates an instance if
	  one does not exist.*/
	static pModelLoader* instance() {
		if (_instance == 0)
		{
			_instance = new pModelLoader();
		}

		return _instance;
	}

	/*ModelInfo struct contains all info
	relevant to the model*/
	pModelLoader();
	~pModelLoader();

	/*Function uses provided path to load a model.
	first it creates an Assimp Importer object,
	uses it to create a scene containing the model to
	be imported. Then a tri-layer loop begins.
	In this loop, the data of every face on every mesh
	belonging to the model is retrieved, converted
	from aiVector3D types to glm::vec3s and
	stored in their respective vectors which are
	contained within the ModelInfo struct.
	The struct is then returned.*/
	pModel* loadModel(std::string path);
private:

	/*contains instance of the pModelLoader object*/
	static pModelLoader* _instance;

	std::vector<GLfloat>* vec3_addToArray(aiVector3D* src, std::vector<GLfloat>* dst);
	std::vector<GLfloat>* vec3_2_addToArray(aiVector3D* src, std::vector<GLfloat>* dst);
	std::vector<GLfloat>* vec4_3_addToArray(aiColor4D* src, std::vector<GLfloat>* dst);
};

