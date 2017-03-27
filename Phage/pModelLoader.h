#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL\glew.h>
#include <glm\glm.hpp>

#include <vector>
#include <string>
#include <map>

#include "pAnimation.h"
#include "pModel.h"
#include "pScene.h"

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

	unsigned int uMatCount;
	unsigned int uMeshCount;

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
	pModel* loadModel(std::string path, pMaterial* mat = nullptr);

	//Loads in resources and returns a tree of scene nodes without a root, meant to be attached to a scene!
	pSceneNode* loadModelToSceneObjects(std::string path, pMaterial* mat = nullptr);

	//TODO: Loads in resources for later access
	void loadModelToResources(std::string path, pMaterial* mat = nullptr);

private:
	//Process node, vertex, and skeletal animation by reading them into resources and associating them with scene nodes
	std::map<std::string, pAnimation*> processAnimations(const aiScene& importScene);
	
	Skeleton* processSkeleton(const aiMesh& mesh);

	//Adds material to the resource manager
	pMaterial* processMaterial(const aiMaterial& material, pShader* shader, const std::string& modelPath, std::string backupName = "");

	pImage* processTextureImage(const aiString& texPath, const std::string& modelPath, const std::string& name = "");

	//Adds model to the resource manager
	pModel* processMesh(const aiMesh& mesh, pMaterial* mat, std::string backupName = "");

	//Creates node tree to put in the pScene
	pSceneNode* processNodes(const aiScene& scene, const std::vector<pModel*> indexedMeshes, const std::map<unsigned int, Skeleton> indexedSkeletons, const std::map <std::string, pAnimation*> animationMap, aiNode* root = nullptr);

	/*contains instance of the pModelLoader object*/
	static pModelLoader* _instance;

	glm::quat aiQuat_to_glmQuat(aiQuaternion aiQuat);

	std::vector<GLfloat>* vec3_addToArray(aiVector3D* src, std::vector<GLfloat>* dst);
	std::vector<GLfloat>* vec3_2_addToArray(aiVector3D* src, std::vector<GLfloat>* dst);
	std::vector<GLfloat>* vec4_3_addToArray(aiColor4D* src, std::vector<GLfloat>* dst);
	glm::mat4 aiMat4_to_glmMat4(aiMatrix4x4 src);
};

