#include "pImageManager.h"


//singleton
pImageManager* pImageManager::instance = nullptr;

//constructor call
pImageManager* pImageManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new pImageManager();
	}
	return instance;
}

//destructor calls clear();
pImageManager::~pImageManager()
{
	clear();
}

//Loads a resource and places it into the item manager, you then can get a pointer by filename or handle
pImageManager::HandleType pImageManager::loadFile(std::string &resName, std::string &fileName)
{
	pImageManager::HandleType result(-1);

	//pImage *img = new FipImage(fileName);
	//result = loadImage(resName, img);

	//delete when we get an image loader working
	result = fakeLoadImage(resName);

	return result;
}

//places a loaded image into the manager
pImageManager::HandleType pImageManager::loadImage(std::string &resName, pImage *image)
{
	pImageManager::HandleType result(-1);

	// check if the resource name is already in use
	result = imageResources.get(resName);
	if (!result.isNull()) {
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Name in use"));
	}
	image->setName(resName);
	result = imageResources.put(resName, image);

	return result;
}

//returns a pointer to an image held by the manager if you input the handle
pImage* pImageManager::getImage(pImageManager::HandleType &handle)
{
	if (handle.isNull()) LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Handle in use",handle.getIndex()));

	pImage *result = imageResources.get(handle);
	if (result == NULL)
	{
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Handle in use", handle.getIndex()));
	}

	return result;
}

//returns a pointer to an image held by the manager if you input the name
pImage* pImageManager::getImage(std::string &nm)
{
	pImage *result = NULL;
	pImageManager::HandleType handle = imageResources.get(nm);
	if (handle.isNull())
	{
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Unknown Name " + nm));
	}
	result = getImage(handle);

	return result;
}

//removes an image from the manager freeing their handle and name
void pImageManager::releaseImage(std::string &nm)
{
	imageResources.remove(nm);
}

//this acts like release image but also deletes the image data
void pImageManager::deleteImage(std::string &nm)
{
	pImage *img = getImage(nm);
	imageResources.remove(nm);
	delete img;
}

//acts like a mass deleteImage
void pImageManager::clear()
{
	imageResources.clearKeysAndValues();
}

//return a handle based in the input name
pImageManager::HandleType pImageManager::nameToHandle(const std::string& name)
{
	return imageResources.get(name);
}

//errorCheck but for image
void pImageManager::errorCheck(std::string& msg)
{
	imageResources.errorCheck(msg);
}

