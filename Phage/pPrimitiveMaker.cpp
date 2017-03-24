#include "pPrimitiveMaker.h"
#include "pResourceFactory.h"
#include "DefaultPaths.h"

pPrimitiveMaker* pPrimitiveMaker::_instance = 0;


pPrimitiveMaker * pPrimitiveMaker::instance()
{
	if (_instance == 0) {
		_instance = new pPrimitiveMaker;
	}

	return _instance;
}


pModel* pPrimitiveMaker::GetPrimitive(std::string name, Primitives type, glm::vec3 scale, pMaterial* material, glm::vec3 color)
{

	if (scale.length() == 0) { 
		scale = glm::vec3(1); 
		LogManager::instance()->warning("Attempted to create primitive with invalid scale\n"); 
	}

	pMaterial* finalMat;
	if (material == nullptr) {
		finalMat = makeBasicMaterial(color, makeBasicShader());
	}
	else {
		finalMat = material;
	}

	switch (type) {
	case Primitives::CUBOID_TRI:
		return pPrimitiveMaker::instance()->makeCuboid_Tri(name, scale, finalMat, color);
	case Primitives::SPHERE_UV:
		return pPrimitiveMaker::instance()->makeSphere_UV(name, scale, finalMat, color);
	case Primitives::PLANE_TRI:
		return pPrimitiveMaker::instance()->makePlane_Tri(name, glm::vec2(scale.x, scale.y), finalMat, color);
	case Primitives::TEAPOT:
		return pPrimitiveMaker::instance()->makeTeapot(name, scale, finalMat, color);
	}
}

pModel * pPrimitiveMaker::makeCuboid_Tri(std::string name, glm::vec3 scale, pMaterial* material, glm::vec3 color)
{
	GLfloat vPositions[] = {
		//Front plane verts
		-scale.x, -scale.y, scale.z, //0
		scale.x, -scale.y, scale.z,
		scale.x, scale.y, scale.z,
		-scale.x, scale.y, scale.z,

		//Right plane verts
		scale.x, scale.y, scale.z, //4
		scale.x, scale.y, -scale.z,
		scale.x, -scale.y, -scale.z,
		scale.x, -scale.y, scale.z,

		//Back plane verts
		-scale.x, -scale.y, -scale.z, //8
		scale.x, -scale.y, -scale.z,
		scale.x, scale.y, -scale.z,
		-scale.x, scale.y, -scale.z,

		//Left plane verts
		-scale.x, -scale.y, -scale.z, //12
		-scale.x, -scale.y, scale.z,
		-scale.x, scale.y, scale.z,
		-scale.x, scale.y, -scale.z,

		//Top plane verts
		scale.x, scale.y, scale.z, //16
		-scale.x, scale.y, scale.z,
		-scale.x, scale.y, -scale.z,
		scale.x, scale.y, -scale.z,

		//Bottom plane verts
		-scale.x, -scale.y, -scale.z, //20
		scale.x, -scale.y, -scale.z,
		scale.x, -scale.y, scale.z,
		-scale.x, -scale.y, scale.z
	};

	GLuint vIndeces[] = {
		//Front
		0, 1, 2,
		0, 2, 3,

		//Right
		4, 5, 6,
		4, 6, 7,
		
		//Back
		8, 9, 10,
		8, 10, 11,

		//Left
		12, 13, 14,
		12, 14, 15,
		
		//Top
		16, 17, 18,
		16, 18, 19,

		//Bottom
		20, 21, 22,
		20, 22, 23
	};

	GLfloat vNormals[] = {
		//Front plane verts
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		//Right plane verts
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		//Back plane verts
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		//Left plane verts
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		//Top plane verts
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		//Bottom plane verts
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0
	};

	GLfloat vTangents[] = {
		//Front plane verts
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		//Right plane verts
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		//Back plane verts
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,

		//Left plane verts
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		//Top plane verts
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		//Bottom plane verts
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1
	};

	GLfloat vCoordinates[] = {
		//Front plane verts
		0, 0,
		1, 0,
		1, 1,
		0, 1,

		//Right plane verts
		0, 1,
		1, 1,
		1, 0,
		0, 0,

		//Back plane verts
		1, 0,
		0, 0,
		0, 1,
		1, 1,

		//Left plane verts
		0, 0,
		1, 0,
		1, 1,
		0, 1,

		//Top plane verts
		1, 0,
		0, 0,
		0, 1,
		1, 1,

		//Bottom plane verts
		1, 0,
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

	};

	pModel* res = new pModel("pCuboid_Tri", material, GL_TRIANGLES, 24, vPositions, 36, vIndeces, vCoordinates, vNormals, vTangents, nullptr, vColors);
	res->directory = "PRIMITIVE_CUBOID_TRI";
	return res;
}

pModel * pPrimitiveMaker::makePlane_Tri(std::string name, glm::vec2 scale, pMaterial* material, glm::vec3 color)
{
	GLfloat vPositions[] = {
		-scale.x, -scale.y, 0,
		-scale.x, scale.y, 0,
		scale.x, -scale.y, 0,
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

	GLfloat vTangents[] = 
	{
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0, 
		0, 1, 0,
		0, 1, 0
	};

	GLfloat vCoordinates[] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};

	GLfloat vColors[] = {
		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z,
		color.x, color.y, color.z
	};

	GLuint vIndeces[] = {
		1, 2, 0,
		1, 3, 2
	};
	
	pModel* res = new pModel("pPlane_Tri", material, GL_TRIANGLES, 4, vPositions, 6, vIndeces, vCoordinates, vNormals, vTangents, nullptr, vColors);
	res->directory = "PRIMITIVE_PLANE_TRI";
	return res;
}

pModel * pPrimitiveMaker::makeSphere_UV(std::string name, glm::vec3 scale, pMaterial * material, glm::vec3 color)
{
	pModel* res = pModelLoader::instance()->loadModel(model_primitive_uvsphere, material);
	return res;
}

pModel * pPrimitiveMaker::makeTeapot(std::string name, glm::vec3 scale, pMaterial * material, glm::vec3 color)
{
	pModel* res = pModelLoader::instance()->loadModel(model_primitive_teapot, material);
	return res;
}

pMaterial * pPrimitiveMaker::makeBasicMaterial(glm::vec3 color, pShader* shader)
{
	pMaterial* res = pResourceFactory::instance()->getMaterial("Default_Primitive_Material");

	if (res != nullptr) {
		return res;
	}

	MaterialInfo basicMatInfo;
	basicMatInfo.diffuse = color;
	basicMatInfo.ambient = glm::vec3(0.1);
	basicMatInfo.specular = glm::vec3(0.2f);
	basicMatInfo.emission = glm::vec3(0);
	basicMatInfo.shininess = 3.0f;
	basicMatInfo.diffuseTexture = NULL;
	basicMatInfo.bumpTexture = NULL;
	basicMatInfo.specularTexture = NULL;

	res = pResourceFactory::instance()->createMaterial("Default_Primitive_Material", shader, basicMatInfo);
	return res;
}

pShader* pPrimitiveMaker::makeBasicShader()
{
	pShader* res = pResourceFactory::instance()->getShader("Default_Primitive_Shader");

	if (res != nullptr) {
		return res;
	}

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

	res = pResourceFactory::instance()->createShader("Default_Primitive_Shader", attribMap, uniMap, shader_primitive_vert, shader_primitive_frag);
	return res;
}