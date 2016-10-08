#pragma once
#include <fstream>

class pFileReader {
public:
	pFileReader();
	~pFileReader();

	char* readFile(char* directory);
	char* convertToChar(std::string in);
private:
	std::ifstream fileStream;
};