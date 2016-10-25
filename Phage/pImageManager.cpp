#include "pImageManager.h"



pImageManager::pImageManager()
{
}


pImageManager::~pImageManager()
{
	clear();
}

pImageManager::HandleType pImageManager::createImage(std::string imageName, std::string filePath)
{
	pImageManager::HandleType result(-1);

	//Create an image based on the name and path
	pImage* img = new pImage(imageName, filePath);
	//Put the new image in the hashtable and return the handle
	return imageResources.put(imageName, img);
}

pImageManager::HandleType pImageManager::addImage(std::string imageName, pImage * image)
{
	return imageResources.put(imageName, image);
}

pImage * pImageManager::getImage(HandleType & handle)
{
	return imageResources.get(handle);
}

pImage * pImageManager::getImage(std::string imageName)
{
	pResourceHandle<pImage> img =  imageResources.get(imageName);
	return imageResources.get(img);
}

void pImageManager::deleteImage(std::string imageName)
{
	imageResources.remove(imageName);
}

void pImageManager::clear()
{
	imageResources.clearKeysAndValues();
}
