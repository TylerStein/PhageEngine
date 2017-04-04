#pragma once
#include "pModel.h"

class pPrimitiveMaker
{
public:
	enum Primitives {CUBOID_TRI,  PLANE_TRI, SPHERE_UV, TEAPOT};
	static pModel* GetPrimitive(std::string name, Primitives type, glm::vec3 scale = glm::vec3(1), pMaterial* material = nullptr, glm::vec3 color = glm::vec3(1));
	static pPrimitiveMaker* instance();
private:
	static pPrimitiveMaker* _instance;

	static pMaterial* makeBasicMaterial(glm::vec3 color, pShader* shader);
	static pShader* makeBasicShader();

	pModel* makeCuboid_Tri(std::string name, glm::vec3 scale, pMaterial* material, glm::vec3 color);
	pModel* makePlane_Tri(std::string name, glm::vec2 scale, pMaterial* material, glm::vec3 color);
	pModel* makeSphere_UV(std::string name, glm::vec3 scale, pMaterial* material, glm::vec3 color);
	pModel* makeTeapot(std::string name, glm::vec3 scale, pMaterial* material, glm::vec3 color);

	pPrimitiveMaker() {};
	~pPrimitiveMaker() {};
};

