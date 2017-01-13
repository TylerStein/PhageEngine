#include "pPrimitiveMaker.h"
#include "pResourceFactory.h"

pPrimitiveMaker* pPrimitiveMaker::_instance = 0;


pPrimitiveMaker * pPrimitiveMaker::instance()
{
	if (_instance == 0) {
		_instance = new pPrimitiveMaker;
	}

	return _instance;
}


pModel* pPrimitiveMaker::GetPrimitive(std::string name, Primitives type, glm::vec3 scale, glm::vec3 color)
{

	if (scale.length() == 0) { 
		scale = glm::vec3(1); 
		LogManager::instance()->warning("Attempted to create primitive with invalid scale\n"); 
	}

	switch (type) {
	case Primitives::CUBOID_QUAD:
		return pPrimitiveMaker::instance()->makeCuboid_Quad(name, scale, color);
	case Primitives::CUBOID_TRI:
		return pPrimitiveMaker::instance()->makeCuboid_Tri(name, scale, color);
	case Primitives::PLANE_QUAD:
		return pPrimitiveMaker::instance()->makePlane_Quad(name, glm::vec2(scale.x, scale.y), color);
	case Primitives::PLANE_TRI:
		return pPrimitiveMaker::instance()->makePlane_Tri(name, glm::vec2(scale.x, scale.y), color);
	}
}

pModel * pPrimitiveMaker::makeCuboid_Tri(std::string name, glm::vec3 scale, glm::vec3 color)
{
	if (scale.length() == 0) { scale = glm::vec3(1); }

	//Create temporary arrays of information representing a 3D cube
	GLfloat vPositions[] = {
		//front
		-scale.x, scale.y, scale.z,
		-scale.x, -scale.y, scale.z,
		scale.x, -scale.y, scale.z,

		-scale.x, scale.y, scale.z,
		scale.x, scale.y, scale.z,
		scale.x, -scale.y, scale.z,

		//right side
		scale.x, -scale.y, scale.z,
		scale.x, -scale.y, -scale.z,
		scale.x, scale.y, -scale.z,

		scale.x, -scale.y, scale.z,
		scale.x, scale.y, scale.z,
		scale.x, scale.y, -scale.z,

		//top
		scale.x, scale.y, -scale.z,
		-scale.x, scale.y, -scale.z,
		scale.x, scale.y, scale.z,

		scale.x, scale.y, scale.z,
		-scale.x, scale.y, scale.z,
		-scale.x, scale.y, -scale.z,

		//back
		-scale.x, scale.y, -scale.z,
		scale.x, scale.y, -scale.z,
		scale.x, -scale.y, -scale.z,

		scale.x, -scale.y, -scale.z,
		-scale.x, -scale.y, -scale.z,
		-scale.x, scale.y, -scale.z,

		//left side
		-scale.x, scale.y, -scale.z,
		-scale.x, scale.y, scale.z,
		-scale.x, -scale.y, scale.z,

		-scale.x, -scale.y, scale.z,
		-scale.x, scale.y, -scale.z,
		-scale.x, -scale.y, -scale.z,

		//bottom
		-scale.x, -scale.y, -scale.z,
		scale.x, -scale.y, -scale.z,
		-scale.x, -scale.y, scale.z,

		-scale.x, -scale.y, scale.z,
		scale.x, -scale.y, scale.z,
		scale.x, -scale.y, -scale.z
	};

	GLfloat vColors [] = {
		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,

		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z
	};
	GLfloat vCoordinates[] = {
		//front
		0, 1,
		0, 0,
		1, 0,

		0, 1,
		1, 1,
		1, 0,

		//right side
		0, 1,
		0, 0,
		1, 0,

		0, 1,
		1, 1,
		1, 0,

		//top
		1, 0,
		0, 0,
		1, 1,

		1, 1,
		0, 1,
		0, 0,

		//back
		0, 1,
		1, 1,
		1, 0,

		1, 0,
		0, 0,
		0, 1,

		//left side
		1, 0,
		1, 1,
		0, 1,

		0, 1,
		1, 0,
		0, 0,

		//bottom
		0, 0,
		1, 0,
		0, 1,

		0, 1,
		1, 1,
		1, 0

	};
	GLfloat vNormals[] = {
		//front
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		//right side
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		//top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		//back
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		//left side
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		//bottom
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
	};

	pShader* basicShader = makeBasicShader();
	pMaterial* basicMat = makeBasicMaterial(color, basicShader);

	return new pModel("pCuboid_Tri", basicMat, GL_TRIANGLES, 36, vPositions, 0, nullptr, vCoordinates, vNormals, nullptr, nullptr, vColors);
}

pModel * pPrimitiveMaker::makeCuboid_Quad(std::string name, glm::vec3 scale, glm::vec3 color)
{
	return nullptr;
}

pModel * pPrimitiveMaker::makePlane_Tri(std::string name, glm::vec2 scale, glm::vec3 color)
{
	if (scale.length() == 0) { scale = glm::vec2(1); }

	GLfloat vPositions[] = {
		-scale.x, -scale.y, 0,
		scale.x, -scale.y, 0,
		scale.x, scale.y, 0,
		-scale.x, -scale.y, 0,
		-scale.x, scale.y, 0,
		scale.x, scale.y, 0
	};

	GLfloat vNormals[] = {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1
	};

	GLfloat vCoordinates[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 0,
		0, 1,
		1, 1
	};

	GLfloat vColors[] = {
		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,
	};

	pShader* basicShader = makeBasicShader();
	pMaterial* basicMat = makeBasicMaterial(color, basicShader);
	pModel* res = new pModel("pPlane_Tri", basicMat, GL_TRIANGLES, 6, vPositions, 0, nullptr, vCoordinates, vNormals, nullptr, nullptr, vColors);
	return res;
}

pModel * pPrimitiveMaker::makePlane_Quad(std::string name, glm::vec2 scale, glm::vec3 color)
{
	return nullptr;
}

pMaterial * pPrimitiveMaker::makeBasicMaterial(glm::vec3 color, pShader* shader)
{
	MaterialInfo basicMatInfo;
	basicMatInfo.diffuse = color;
	basicMatInfo.ambient = glm::vec3(0.1);
	basicMatInfo.specular = glm::vec3(0.2f);
	basicMatInfo.emission = glm::vec3(0);
	basicMatInfo.shininess = 3.0f;
	basicMatInfo.diffuseTexture = NULL;
	basicMatInfo.bumpTexture = NULL;
	basicMatInfo.specularTexture = NULL;

	pMaterial* res = new pMaterial("PrimitiveMaterial", shader, basicMatInfo);
	return res;
}

pShader* pPrimitiveMaker::makeBasicShader()
{
	attribNameMap attribMap = attribNameMap();
	attribMap.insert(Attributes::VertexPosition, "vPosition");
	attribMap.insert(Attributes::VertexNormal, "vNormal");
	attribMap.insert(Attributes::VertexCoordinate, "vCoordinate");
	attribMap.insert(Attributes::VertexColor, "vColor");

	uniformNameMap uniMap = uniformNameMap();
	uniMap.insert(Uniforms::DiffuseColor, "diffuseColor");
	uniMap.insert(Uniforms::Camera_View, "cameraView");
	uniMap.insert(Uniforms::Model_View, "modelView");
	uniMap.insert(Uniforms::Projection_View, "projectionView");

	pShader* res = new pShader("PrimitiveShader", attribMap, uniMap, "../Resources/Shaders/Primitive.vert", "../Resources/Shaders/Primitive.frag");
	return res;
}