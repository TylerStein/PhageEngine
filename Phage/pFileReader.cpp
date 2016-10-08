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

char* pFileReader::readFile(char * directory)
{
	//Open the file at the directory for reading input
	fileStream.open(directory, std::ios::in);

	//Make sure the filestream is functional
	if (!fileStream.is_open() || !fileStream.good()) {
		std::printf("Could not open file from directory: %s", directory);
		return NULL;
	}

	std::string out;
	std::string readLine;
	//Read in each line, add a newline at the end of each
	while (std::getline(fileStream, readLine)) {
		out += readLine + "\n";
	}

	//Close the istream
	fileStream.close();

	//Return the result as a char*
	return convertToChar(out);
}

char * pFileReader::convertToChar(std::string in)
{
	//Memory-leak-free and exception-safe method!

	//Store the input string as a vector of chars
	std::vector<char> out(in.begin(), in.end());
	//Add the end of character array identifier
	out.push_back('\0');

	return &out[0];
}
