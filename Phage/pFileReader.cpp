#include "pFileReader.h"
#include <iostream>
#include <string>
#include <vector>

pFileReader::pFileReader()
{

}

pFileReader::~pFileReader()
{

}

std::string pFileReader::readFile(char * directory)
{
	//Open the file at the directory for reading input
	fileStream.open(directory, std::ios::in);

	//Make sure the filestream is functional
	if (!fileStream.is_open() || !fileStream.good()) {
		std::printf("Could not open file from directory: %s", directory);
		return "NULL_FILE";
	}

	std::string out;
	std::string readLine;
	//Read in each line, add a newline at the end of each
	while (std::getline(fileStream, readLine)) {
		out += readLine + "\n";
	}

	//Close the istream
	fileStream.close();

	//Return the result
	return out;
}