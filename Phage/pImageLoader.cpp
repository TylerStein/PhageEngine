#include "pImageLoader.h"
#include "FreeImage.h"
#include "LogManager.h"

ImageData pImageLoader::loadImage(char * filePath)
{
	//Detect the image format
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePath, 0);
	//Load in the image
	FIBITMAP* image = FreeImage_Load(format, filePath);
	//Convert image to 24-bit format (RGB)
	//image = FreeImage_ConvertTo32Bits(image);
	image = FreeImage_ConvertTo24Bits(image);
	//Get the image width
	GLuint width = FreeImage_GetWidth(image);
	//Get the image height
	GLuint height = FreeImage_GetHeight(image);

	//Check if the width and height is a power of  (can handle this in some way later)
	if ((width != 0 && height != 0)) {
		if ((width / height) % 2 != 1) {
			//Not a power of 2!
			LogManager::instance()->warning("Image loaded is not a power of 2!");
		}
	}
	else {
		LogManager::instance()->warning("Image loaded has a 0 width or height!");
	}

	//Return the image data bits
	GLubyte* imageData = (GLubyte*)malloc(width * height * 3 * sizeof(GLfloat));
	imageData = FreeImage_GetBits(image);

	return ImageData(imageData, width, height);
}