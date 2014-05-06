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
		}

	};
	
	struct dirEntry{
		string symbolic_file_name;
		int indexForDesc;
	};

	dirEntry directory[];
	FileSystem::OFT openFileTable[3];
	IOSystem* iosystem;

public:
	FileSystem::FileSystem(IOSystem* iosystem);
	int open(std::string symbolicName);
	int open_desc(int desc_no);
	void close(int index);

};

#endif