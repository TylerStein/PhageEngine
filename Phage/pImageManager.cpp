#include "pImageManager.h"



pImageManager::pImageManager()
{
}


pImageManager::~pImageManager()
{
	clear();
}

pImageManager::HandleType pImageManager::createImage(char * imageName, char * filePath)
{
	pImageManager::HandleType result(-1);

	//Create an image based on the name and path
	pImage* img = new pImage(imageName, filePath);
	//Put the new image in the hashtable and return the handle
	return imageResources.put((std::string)imageName, img);
}

pImageManager::HandleType pImageManager::addImage(char * imageName, pImage * image)
{
	return imageResources.put((std::string)imageName, image);
}

pImage * pImageManager::getImage(HandleType & handle)
{
	return imageResources.get(handle);
}

pImage * pImageManager::getImage(char * imageName)
{
	pResourceHandle<pImage> img =  imageResources.get(imageName);
	return imageResources.get(img);
}

void pImageManager::deleteImage(char * imageName)
{
	imageResources.remove((std::string)imageName);
}

void pImageManager::clear()
{
	imageResources.clearKeysAndValues();
}
