#include "pCubeMap.h"
#include "pResourceFactory.h"

GLenum cubeMapSides[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

pCubeMap::pCubeMap(const std::string & file_posX, const std::string & file_negX, const std::string & file_posY, const std::string & file_negY, const std::string & file_posZ, const std::string & file_negZ)
{
	//Copy the contents of the temporary array to the _filePaths member variable array
	_filePaths = std::vector<std::string>();

	_filePaths.push_back(file_posX);
	_filePaths.push_back(file_negX);

	//TODO: Out of order? 
	_filePaths.push_back(file_negY);
	_filePaths.push_back(file_posY);

	_filePaths.push_back(file_posZ);
	_filePaths.push_back(file_negZ);

	LoadImages();
	LoadModel();
}

pCubeMap::~pCubeMap()
{
}

bool pCubeMap::LoadImages()
{
	//Use the first image for width and height, get it's data pointer
	ImageData tmpData = pImageLoader::loadImage((char*)_filePaths[0].c_str());
	_imageWidth = tmpData._width;
	_imageHeight = tmpData._height;
	_imageData[0] = tmpData._data;

	//Make sure images are valid
	if (_imageWidth * _imageHeight == 0) { return false; }

	for (int i = 1; i < 6; ++i) {
		//Get the data pointers for the rest of the cubemap images
		_imageData[i] = pImageLoader::loadImage((char*)_filePaths[i].c_str())._data;
	}

	//Create the OpenGL texture
	glGenTextures(1, &_cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);

	//Set up each image in the cubemap for their corresponding sides (pX, nX, pY, nY, pZ, nZ)
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(cubeMapSides[i], 0, GL_RGB, _imageWidth, _imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, _imageData[i]);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	

	return true;
}

void pCubeMap::LoadModel()
{
	pMaterial* skyMat = pResourceFactory::instance()->createMaterial("SkyMaterial", pResourceFactory::instance()->createSkyboxShader(), MaterialInfo());

	_model = pPrimitiveMaker::instance()->GetPrimitive("SkyCube", pPrimitiveMaker::CUBOID_TRI, glm::vec3(20.0f), skyMat, glm::vec3(1));
	_transform = glm::mat4();
	_transform = glm::rotate(_transform, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}

void pCubeMap::UseTexture(GLenum texNum)
{
	glActiveTexture(texNum);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);
}

void pCubeMap::UnuseTexture(GLenum texNum)
{
	glActiveTexture(texNum);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(0);
}

glm::mat4 pCubeMap::getTransform()
{
	return _transform;
}

pShader * pCubeMap::getShader()
{
	return _model->getMaterial()->getShader();
}

pModel * pCubeMap::getModel()
{
	return _model;
}

pMaterial * pCubeMap::getMaterial()
{
	return _model->getMaterial();
}