#include "pModel.h"
#include "pFileReader.h"
#include <iostream>
#include <vector>

pModel::pModel(char* name, pMaterial* material, GLfloat* verts, GLuint numVertices)
	:pAsset(pType::MODEL)
{
	this->name = name;
	this->type = pType::MODEL;
	this->material = material;
	vertCount = numVertices;

	//Allocate the required memory for the model's vertices (each vertex is 3 floats, so #verts * 3 * size of a float)
	//Could be made more convenient with a vertex struct
	vertices = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

	//Copy over the verticies array (numVertices * 3 because there are 3x locations per vertex, again would be better with a vertex struct)
	for (GLuint x(0); x < (numVertices * 3); ++x) {
		vertices[x] = verts[x];
	}

	setupModel();
}

pModel::~pModel()
{
	delete vertices;
}

std::string pModel::getName()
{
	return name;
}

GLuint pModel::getID()
{
	return ID;
}

void pModel::setupModel()
{

	pFileReader reader;

	//Read the shader files
	std::string vs = reader.readFile("../Resources/Shaders/simpleVertexShader.vert");
	std::string fs = reader.readFile("../Resources/Shaders/simpleFragmentShader.frag");

	//Create char* of the shaders
	const char* vertex_shader = vs.c_str();
	const char* fragment_shader = fs.c_str();

	//Create the vertex buffer object
	vertexBufferID = 0;
	//Generate the vbo
	glGenBuffers(1, &vertexBufferID);
	//Set buffer as the current one by binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	//Copy the points into the bound buffer
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Create vertex array object
	vertexArrayID = 0;
	//Generate the vao
	glGenVertexArrays(1, &vertexArrayID);
	//Set the vao as the current one
	glBindVertexArray(vertexArrayID);
	//Only one vertex buffer, so attribute 0
	glEnableVertexAttribArray(0);
	//Bind that buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayID);
	//Define the layout for attribute number 0,
	//3 means the variables are vec3 made from 3 floats in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Load the shaders
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vertex_shader, NULL);
	glCompileShader(vertShaderID);

	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fragment_shader, NULL);
	glCompileShader(fragShaderID);

	//Create the shader program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, fragShaderID);
	glAttachShader(shaderProgramID, vertShaderID);
	glLinkProgram(shaderProgramID);
}
