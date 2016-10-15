#pragma once
#include "GL\glew.h"
#include <string>
#include "pAsset.h"
#include "pMaterial.h"

class pModel : public pAsset
{
public:
	pModel(char* name, pMaterial* material, GLfloat* verts, GLuint numVertices);
	~pModel();

	std::string getName() override;
	GLuint getID() override;

	GLuint vertexBufferID;
	GLuint vertexArrayID;
	GLuint shaderProgramID;

	std::string name;
	pType type;
	pMaterial* material;
	GLuint vertCount;
	GLfloat* vertices;
private:
	//Function for initializing the VBO, VAO and ShaderProgram
	void setupModel();

};

