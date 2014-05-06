#include "stdafx.h"
#include "FileSystem.hpp"
#include <iostream>
#include <string>

using namespace std;

FileSystem::FileSystem(IOSystem* iosystem){

	this->iosystem = iosystem;

	for(int i = 0; i < 14; i ++){
		dir[i] = dirEntry();
	}
	for(int i = 0; i < 3; i ++){
		openFileTable[i] = OFT();
	}
}

int FileSystem::open(std::string symbolicName){
	for (int i = 0; i < 14; i++){
		bool match = true;
		for (int j = 0; symbolicName[j] != 0; j++){		
			if (dir[i].symbolic_file_name[j] != symbolicName[j]){
				match = false;
				break;
			}
		}
		if (match){
			bool spaceAvailable = false;
			for (int j = 0; j < 3; j++){
				if (openFileTable[j].isEmpty()){
					openFileTable[j].currentPosition = 0;
					openFileTable[j].fileDescriptorIndex = i;
					iosystem->read_block(i, 0);
					for (int k = 0; k < 64; k++){
						openFileTable[j].bufferReader[k] = iosystem->getCurrentBlock()[k];
					}
					spaceAvailable = true;
				}
			}
			if (!spaceAvailable){
				return -1;
			}
			return i;
		}
	}
	return -1;
} 

int FileSystem::open_desc(int file_no){
	if (file_no > 13){
		return -1;
	}
	for (int i = 0; i < 3; i++){
		if (openFileTable[i].isEmpty()){
			openFileTable[i].currentPosition = 0;
			openFileTable[i].fileDescriptorIndex = file_no;
			iosystem->read_block(i, 0);
			for (int j = 0; j < 64; j++){
				openFileTable[i].bufferReader[j] = iosystem->getCurrentBlock()[j];
			}
			return file_no;
		}
	}
	return -1;
}

void FileSystem::close(int index){
	if (openFileTable[index].isEmpty()){
		return;
	}
	openFileTable[index] = OFT();
}

void FileSystem::directory(){
	for (int i=0; i < 14; i++){
		dirEntry entry = dir[i];
		if (&entry){
			char* fileDesc = new char[64];
			iosystem->read_block(entry.indexForDesc, fileDesc);
			std::cout << entry.symbolic_file_name << " " << fileDesc[0] << std::endl;
		}
	}
}

