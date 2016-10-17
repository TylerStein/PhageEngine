#pragma once
//#include "pImage.h"
#include "pResourceManager.h"
class pImageManager
{
public:
	//this is the handle used for resources
	typedef ResourceHandle<pImage> HandleType;

private:
	ResourceManager<pImage>	imageResources;
	static pImageManager* instance;

	//constructor for an empty manager
	pImageManager() {}
public:

	//singleton because you never want more than one manager
	static pImageManager* getInstance();

	//destructor which will delete all of the resources held by the manager
	~pImageManager();

	//Loads a resource and places it into the item manager, you then can get a pointer by filename or handle
	pImageManager::HandleType loadFile(std::string &resName, std::string &fileName);

	//places a loaded image into the manager
	pImageManager::HandleType loadImage(std::string &resName, pImage *image);

	//delete this when we get the imageloader working
	pImageManager::HandleType fakeLoadImage(std::string &resName);

	//returns a pointer to an image held by the manager if you input the handle
	pImage* getImage(pImageManager::HandleType &handle);

	//returns a pointer to an image held by the manager if you input the name
	pImage* getImage(std::string &nm);

	//removes an image from the manager freeing their handle and name
	void releaseImage(std::string &nm);

	//this acts like release image but also deletes the image data
	void deleteImage(std::string &nm);

	//acts like a mass deleteImage
	void clear();

	//return a handle based in the input name
	pImageManager::HandleType nameToHandle(const std::string& name);

	//errorCheck but for image
	void errorCheck(std::string& msg);

};
};

