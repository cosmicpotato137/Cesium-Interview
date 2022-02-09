#include "read_data.h"
#include <iostream>

using namespace std;

char* readData(const std::string& s)
{
	std::ifstream file;
	size_t size = 0;

	std::cout << "Attempting to open " << s << std::endl;

	file.open(s, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file)
		std::cout << "file not found" << std::endl;
	char* data = 0;

	file.seekg(0, std::ios::end);
	size = file.tellg();
	std::cout << "File size: " << size << std::endl;
	file.seekg(0, std::ios::beg);

	data = new char[size];
	//file.seekg(8); // skip the header
	file.read(data, size);
	return data;
}