#pragma once
#include "GL\glew.h"
#include <string>
#include "pAsset.h"
#include "pMaterial.h"
#include "glm\mat4x4.hpp"

class pModel : public pAsset
{

	friend class pBoundingBox;

public:
	pModel(char* name, pMaterial* material, GLfloat* verts, GLfloat* vertColors, GLfloat* vertUVs, GLuint numVertices);
	~pModel();

	std::string getName() override;
	GLuint getID() override;

	GLuint getShaderProgramID();
	GLuint getVertCount();
	GLuint getVertexArrayID();
	GLuint getVertexBufferID(GLuint index);
	GLuint getModelMatrixID();
	GLuint getViewMatrixID();
	GLuint getProjectionMatrixID();

	glm::mat4 getModelMatrix();
	pMaterial* getMaterial();

	//Adds rotation relative to current around a provided axis by <amount> degrees
	void rotateAround(glm::vec3 rot, GLfloat amount);
	//Scales relative to current scale by <xyz>
	void scale(glm::vec3 scl);
	//Sets the scale (not relative) by <xyz>
	void setScale(glm::vec3 scl);
	//Translates relative to current position by <xyz>
	void translate(glm::vec3 pos);
	//Translates relative to center by <xyz>
	void setPosition(glm::vec3 pos);

	pType type;

private:
	//Function for initializing the VBO, VAO and ShaderProgram
	void setupModel();
	void initDefaultMatrix();

	pMaterial* material;
	GLuint vertCount;
	GLfloat* vertices;
	GLfloat* vertexColors;
	GLfloat* vertexUVs;
	GLuint VBOID[3]; //3 vertex buffer objects, for points, colors, and UVs
	GLuint VAOID; //One vertex array object to hold the vertex buffer objects
	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	glm::mat4 modelMatrix;

	std::string name;
	GLuint ID;
};

