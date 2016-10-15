#pragma once
#include <fstream>

class pFileReader {
public:
	pFileReader();
	~pFileReader();

	std::string readFile(char* directory);
private:
	std::ifstream fileStream;
};