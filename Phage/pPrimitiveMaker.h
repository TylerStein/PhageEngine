#pragma once
#include "pModel.h"

class pPrimitiveMaker
{
public:
	static enum Primitives {CUBOID_TRI, CUBOID_QUAD, PLANE_TRI, PLANE_QUAD};
	static pModel* GetPrimitive(std::string name, Primitives type, glm::vec3 scale = glm::vec3(1), glm::vec3 color = glm::vec3(1));
	static pPrimitiveMaker* instance();
private:
	static pPrimitiveMaker* _instance;

	static pMaterial* makeBasicMaterial(glm::vec3 color, pShader* shader);
	static pShader* makeBasicShader();

	pModel* makeCuboid_Tri(std::string name, glm::vec3 scale, glm::vec3 color);
	pModel* makeCuboid_Quad(std::string name, glm::vec3 scale, glm::vec3 color);

	pModel* makePlane_Tri(std::string name, glm::vec2 scale, glm::vec3 color);
	pModel* makePlane_Quad(std::string name, glm::vec2 scale, glm::vec3 color);

	pPrimitiveMaker() {};
	~pPrimitiveMaker() {};
};

