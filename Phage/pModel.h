#pragma once
#include "GL\glew.h"
#include <string>
#include "pAsset.h"
#include "pMaterial.h"

class pModel : public pAsset
{
public:
	pModel(char* name, pMaterial* material, GLfloat* verts, GLfloat* vertColors, GLuint numVertices);
	~pModel();

	std::string getName() override;
	GLuint getID() override;

	GLuint getShaderProgramID();
	GLuint getVertCount();
	GLuint getVertexArrayID();
	GLuint getVertexBufferID();

	pType type;

private:
	//Function for initializing the VBO, VAO and ShaderProgram
	void setupModel();
	pMaterial* material;
	GLuint vertCount;
	GLfloat* vertices;
	GLfloat* vertexColors;
	GLuint vertexBufferID;
	GLuint vertexArrayID;
	std::string name;
};

