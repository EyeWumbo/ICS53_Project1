#include "stdafx.h"
#include "FileSystem.hpp"
#include <iostream>
#include <string>

using namespace std;

FileSystem::FileSystem(IOSystem* iosystem){

	this->iosystem = iosystem;

	for(int i = 0; i < 14; i ++){
		dir[i] = nullptr;
	}
	for(int i = 0; i < 3; i ++){
		openFileTable[i] = OFT();
	}
}

/* int FileSystem::open(std::string symbolicName){
	for (int i = 0; i < 14; i++){
		for (int j = 0; symbolicName[j] != 0; j++){
			if ()
		}
	}
	return -1;
} */

void FileSystem::directory(){
	for (int i=0; i < 14; i++){
		dirEntry* entry = dir[i];
		if (entry){
			char* fileDesc = new char[64];
			iosystem->read_block(entry->indexForDesc, fileDesc);
			std::cout << entry->symbolic_file_name << " " << fileDesc[0] << std::endl;
		}
	}
}

