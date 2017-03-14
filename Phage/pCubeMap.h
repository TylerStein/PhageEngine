#pragma once
#include "GL/glew.h"
#include "pImageLoader.h"
#include "glm/mat4x4.hpp"
#include "pShader.h"
#include "pMaterial.h"
#include "pModel.h"
#include <string>

class pCubeMap
{
public:
	pCubeMap(const std::string& file_posX, 
		const std::string& file_negX,
		const std::string& file_posY,
		const std::string& file_negY,
		const std::string& file_posZ,
		const std::string& file_negZ);

	~pCubeMap();

	bool LoadImages();
	void LoadModel();

	void UseTexture(GLenum texNum);
	void UnuseTexture(GLenum texNum);

	pShader* getShader();
	pModel* getModel();
	pMaterial* getMaterial();
private:
	//Paths to image files
	std::vector<std::string> _filePaths;

	//Pointers to raw image data
	GLubyte* _imageData[6];

	//All images assumed to have matching width/height
	GLuint _imageWidth;
	GLuint _imageHeight;

	//OpenGL cubemap texture ID
	GLuint _cubeMapID;

	//Pointer to attatched model
	pModel* _model;

	//Skybox transformation matrix
	glm::mat4 _transform;

};

