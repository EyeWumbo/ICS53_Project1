//============================================================================
// Author      : Leonard Bejosano, Melody Truong, TC Nguyen, Steven Zhang
// Version     : 0.1.2.1.2a
// Copyright   : Your copyright notice
// Description : First Project Lab
//============================================================================

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "IOSystem.hpp"

class FileSystem{

private:
	struct OFT{

		char bufferReader[64];
		int currentPosition;
		int fileDescriptorIndex;

		OFT(){
			currentPosition = -1;
			fileDescriptorIndex = -1;
			for (int i = 0; i < 64; i++){
				bufferReader[i] = 0;
			}
		}

		bool isEmpty(){
			return currentPosition == -1;
		}

	};
	
	struct dirEntry{
		std::string symbolic_file_name;
		int descriptorIndex;

		dirEntry(std::string name, int index)
		{
			symbolic_file_name = name;
			descriptorIndex = index;
		}

		dirEntry(){
			symbolic_file_name = "";
			descriptorIndex = -1;
		}

	};

	OFT openFileTable[3];
	IOSystem* iosystem;
	char tempBuffer[64];

	unsigned char convertSize(const char arg);

public:
	FileSystem(IOSystem* iosystem);
	int open(std::string symbolicName);
	int open_desc(int desc_no);
	void close(int index);
	void directory();
	int create(std::string symbolic_file_name);
	int deleteFile(std::string fileName);
	int read(int index, char* mem_area, int count);
};

#endif