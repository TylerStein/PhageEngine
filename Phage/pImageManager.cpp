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
	result =  imageResources.put(imageName, img);
	return result;
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

std::vector<pImage*> pImageManager::getAllImages()
{
	auto inList = imageResources.getAll();
	std::vector<pImage*> outList = std::vector<pImage*>();
	for (int i = 0; i < inList.size(); ++i) {
		outList.push_back(
			imageResources.get(
				inList.at(i)
			));
	}

	return outList;
}

void pImageManager::deleteImage(std::string imageName)
{
	imageResources.remove(imageName);
}

void pImageManager::clear()
{
	imageResources.clearKeysAndValues();
}
