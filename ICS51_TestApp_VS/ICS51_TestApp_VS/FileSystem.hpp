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
		int indexForDesc;
	};

	dirEntry dir[14];
	OFT openFileTable[3];
	IOSystem* iosystem;

public:
	FileSystem(IOSystem* iosystem);
	int open(std::string symbolicName);
	int open_desc(int desc_no);
	void close(int index);
	void directory();

};

#endif