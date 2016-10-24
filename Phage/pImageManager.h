#pragma once
#include "pResourceManager.h"
#include "pImage.h"
#include "pImageLoader.h"

class pImageManager
{
public:
	typedef pResourceHandle<pImage> HandleType;

	pImageManager();
	~pImageManager();

	//Gets a material from file on disk
	HandleType createImage(char* imageName, char* filePath);

	//Adds a material to the manager
	HandleType addImage(char* imageName, pImage* image);

	//Retreives a material from the manager
	pImage* getImage(HandleType &handle);
	pImage* getImage(char* imageName);

	//Delete a material from the manager
	void deleteImage(char* imageName);

	//Clear all materials from the manager
	void clear();

private:
	pResourceManager<pImage> imageResources;
};

