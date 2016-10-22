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
	/*Constructor
	  Takes in the path to the model, uses it to
	  call @func loadModel*/
	pModelLoader(GLchar* path);

	/*Getters:
	  @func getPositions returns the vector containing position vertices
	  @func getUVs returns the vector containing UV data
	  @func getNormals returns the vector containing normal data*/
	std::vector<glm::vec3> getPositions();
	std::vector<glm::vec3> getUVs();
	std::vector<glm::vec3> getNormals();

private:
	/*Function uses provided path to load a model.
	  first it creates an Assimp Importer object,
	  uses it to create a scene containing the model to
	  be imported. Then a tri-layer loop begins.
	  In this loop, the data of every face on every mesh
	  belonging to the model is retrieved, converted
	  from aiVector3D types to glm::vec3s and 
	  stored in their respective vectors.*/
	void loadModel(std::string path);

	/*Storage Vectors
	  @var positions stores model position vertices
	  @var uvs stores the model uv vectors
	  @var normals stores the model normal vectors*/
	std::vector<glm::vec3> *positions;
	std::vector<glm::vec3> *uvs;
	std::vector<glm::vec3> *normals;

};

