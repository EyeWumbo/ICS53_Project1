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

			int OFTIndex; //For purposes of finding the OFT index most recently added to.

			for (int j = 0; j < 3; j++){
				if (openFileTable[j].isEmpty()){
					openFileTable[j].currentPosition = 0;
					openFileTable[j].fileDescriptorIndex = i;
					iosystem->read_block(i, openFileTable[j].bufferReader);
					spaceAvailable = true;
					OFTIndex = j;
					break;
				}
			}
			if (!spaceAvailable){
				std::cout << "No space available in the OFT (All 3 spots taken)" << std::endl;
				return -1;
			}

			std::cout << "File of name " << symbolicName << " opened at OFT position " << OFTIndex << std::endl;
			return OFTIndex;
		}
	}
	std::cout << "No file matching name " << symbolicName << " found." << std::endl;
	return -1;
} 

int FileSystem::open_desc(int file_no){
	if (file_no > 13){
		std::cout << "The input " << file_no << " is greater than the number of possible files, which is 14" << std::endl;
		return -1;
	}
	if (dir[file_no].isEmpty()){
		std::cout << "Attempting to open empty file at descriptor index " << file_no << " is a no-no." << std::endl;
		return -1;
	}
	for (int i = 0; i < 3; i++){
		if (openFileTable[i].isEmpty()){
			openFileTable[i].currentPosition = 0;
			openFileTable[i].fileDescriptorIndex = file_no;
			iosystem->read_block(i, openFileTable[i].bufferReader);
			std::cout << "File descriptor at position " << file_no << " written at OFT position " << i << std::endl;
			return i;
		}
	}
	std::cout << "No empty OFT positions available when attempting to open descriptor " << file_no << std::endl;
	return -1;
}

void FileSystem::close(int index){

	//TODO
	//Write buffer here.
	//Update filesize here.

	for (int i = 0; i < 3; i++){
		if (openFileTable[i].fileDescriptorIndex == index){
			iosystem->write_block(openFileTable[i].fileDescriptorIndex, openFileTable[i].bufferReader);
			openFileTable[i] = OFT();
			std::cout << "OFT at index " << index << " closed." << std::endl;
			return;
		}
	}

	std::cout << "File descriptor at index " << index << " not found in the OFT." << std::endl;
	
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

int FileSystem::create(std::string symbolic_file_name){
	char current[64];
	for (int i = 0; i < 14; ++i){
		dirEntry currentDirEntry = dir[i];
		if (currentDirEntry.symbolic_file_name.compare(symbolic_file_name) == 0){ 
			std::cout << "File " << symbolic_file_name << " already exists." << std::endl;
			return -2;
		}	
	}
	int descriptorIndex = iosystem->findFreeDescriptor();
	
	if (descriptorIndex == -1){ std::cout << "No empty file descriptors found." << std::endl; return -1; }
		

	for(int i = 0; i < 14; i++)
	{
		if(dir[i].isEmpty()){
			dir[i] = dirEntry(symbolic_file_name, descriptorIndex);
			std::cout << "File " << symbolic_file_name << " written at position " << descriptorIndex << std::endl;
			break;
		}
	}

	return 0;
}

int FileSystem::deleteFile(std::string fileName){
	int descriptorIndex = -1;

	for (int i = 0; i < 14 && descriptorIndex == -1; ++i){
		dirEntry currentDirEntry = dir[i];
		if(currentDirEntry.symbolic_file_name.compare(fileName) == 0)
		{
			descriptorIndex = currentDirEntry.descriptorIndex;
		}
	}
	
	if (descriptorIndex == -1){
		std::cout << "No file of name " << fileName << " found." << std::endl;
		return -1;
	}
		

	//update bit map

	iosystem->freeFileDescriptor(descriptorIndex);
	dir[descriptorIndex] = dirEntry();
	std::cout << "File of name " << fileName << " at position " << descriptorIndex << " was deleted." << std::endl;
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
    
	for (int i = 0; i < count; i++){
		openFileTable[index].bufferReader[openFileTable[index].currentPosition] = value;
	}
    
    return 1;
}

int FileSystem::lseek(int index, int pos)
{
    int position = pos/64;
    iosystem->read_block(index + position, openFileTable[index].bufferReader);
    openFileTable[index].currentPosition = pos-64*position;
    
    return 1;
}
