#include "stdafx.h"
#include "FileSystem.hpp"
#include <iostream>
#include <string>

using namespace std;

FileSystem::FileSystem(IOSystem* iosystem){

	this->iosystem = iosystem;

	for(int i = 0; i < 14; i ++){
		dir[i] = dirEntry("", -1);
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
			iosystem->read_block(entry.descriptorIndex, fileDesc);
			std::cout << entry.symbolic_file_name << " " << fileDesc[0] << std::endl;
		}
	}
}

int FileSystem::create(string symbolic_file_name){
	char* current = new char[64];

	for (int i = 0; i < 14; ++i){
		dirEntry currentDirEntry = dir[i];
		if(currentDirEntry.symbolic_file_name.compare(symbolic_file_name) == 0)
			return -2;
	}

	int descriptorIndex = iosystem->findFreeDescriptor();
	dirEntry* currentDirEntry = nullptr;
	
	if(descriptorIndex != -1)
		return -1;

	for(int i = 0; i < 14 && currentDirEntry == nullptr; i++)
	{
		//if(dir[i] == nullptr){
		//	currentDirEntry = new dirEntry(symbolic_file_name, descriptorIndex);
		//	dir[i] = d;
		//}

		currentDirEntry = new dirEntry(symbolic_file_name, descriptorIndex);
		dir[i] = *currentDirEntry;

	}
	
	return 0;
}

int FileSystem::deleteFile(string fileName){
	int descriptorIndex = -1;

	for (int i = 0; i < 14 && descriptorIndex == -1; ++i){
		dirEntry currentDirEntry = dir[i];
		if(currentDirEntry.symbolic_file_name.compare(fileName) == 0)
		{
			descriptorIndex = currentDirEntry.descriptorIndex;
			//delete currentDirEntry;
			dir[i] = dirEntry("", -1);
		}
	}
	
	if(descriptorIndex == -1)
		return -1;

	//update bit map

	iosystem->freeFileDescriptor(descriptorIndex);
	return 0;
}

int FileSystem::read(int index, char* mem_area, int count)
{
    
    iosystem->read_block(index, mem_area);
    
    if(open_desc(index) == -1)
    {
        return -1;        
    }
    else
    {
        int some_variable = open_desc(index);
        openFileTable[some_variable];
        
        return 1;
    }
}

int FileSystem::write(int index, char value, int count)
{
    iosystem->write_block(i, openFileTable[index].bufferReader);
    for(int i=0; i<64; i++)
    {
        currentBlock[i] = bufferReader[i];
    }
    
    return 1;
}

int FileSystem::lseek(int index, int pos)
{
    int position = pos/64;
    iosystem->read_block(index + position);
    openFileTable[index].currentPosition = pos-64*position;
    
    return 1;
}
