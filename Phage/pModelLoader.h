#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <string>

class pModelLoader
{
public:
	/*Function returns an instance of the
	  pModelLoader object. Creates an instance if
	  one does not exist.*/
	static pModelLoader* instance();

	/*ModelInfo struct contains all info
	relevant to the model*/
	struct ModelInfo {
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> uvs;
		std::vector<glm::vec3> normals;
	};

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
	ModelInfo loadModel(std::string path);
private:
	pModelLoader();
	~pModelLoader();

	/*contains instance of the pModelLoader object*/
	static pModelLoader* _instance;

	

};

