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

	iosystem->read_block(1, openFileTable[0].bufferReader);
	

}

int FileSystem::open(std::string symbolicName){

	int tableIndex = -1;

	for (int i = 0; i < 3; i++){
		if (openFileTable[i].isEmpty()){
			tableIndex = i;
		}
	}

	if (tableIndex == -1){
		return -1;
	}

	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		if (tempBuffer[i] == 0){
			return -1;
		}
		iosystem->read_block(tempBuffer[i], tempBuffer);
		for (int j = 0; j < 64; j+= 11){
			bool match = true;
			for (int k = 0; symbolicName[k] != 0; k++){
				if (k == 10){
					break;
				}
				if (tempBuffer[j + k] != symbolicName[k]){
					match = false;
					break;
				}
			}
			if (match){
				openFileTable[tableIndex].fileDescriptorIndex = tempBuffer[j + 10];
				int tempPosition = tempBuffer[j + 10] / 6 * 4;
				iosystem->read_block(tempBuffer[j + 10] % 6 + 1, tempBuffer);
				iosystem->read_block(tempBuffer[tempPosition + 1], openFileTable[tableIndex].bufferReader);
				return 1;
			}
		}

	}

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
	//Update filesize here.

	/*for (int i = 0; i < 3; i++){
		if (openFileTable[i].fileDescriptorIndex == index){
			iosystem->write_block(openFileTable[i].fileDescriptorIndex, openFileTable[i].bufferReader);
			openFileTable[i] = OFT();
			std::cout << "OFT at index " << index << " closed." << std::endl;
			return;
		}
	}

	std::cout << "File descriptor at index " << index << " not found in the OFT." << std::endl;*/

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
	iosystem->read_block(1, tempBuffer);
	char fileDescriptorIndex = 0;
	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		if (tempBuffer[i] == 0){
			break;
		}
		iosystem->read_block(tempBuffer[i], tempBuffer);
		for (int k = 0; k < 64; k += 11){
			if (tempBuffer[k] == 0){
				break;
			}
			bool match = true;
			for (int j = 0; symbolic_file_name[j] != 0; j++){
				if (j == 10){
					break;
				}
				if (tempBuffer[k + j] != symbolic_file_name[j]){
					match = false;
					break;
				}
			}
			if (match){
				std::cout << "File " << symbolic_file_name << " already exists." << std::endl;
				return -1;
			}
		}
	}
	for (int i = 1; i < 14; i++){
		iosystem->read_block((i % 6) + 1, tempBuffer);
		int tempPosition = i / 6 * 4;
		if (tempBuffer[tempPosition] == 0){
			fileDescriptorIndex = i;
			break;
		}
	}
	if (fileDescriptorIndex == 0){
		std::cout << "No files open" << std::endl;
		return -1;
	}
	iosystem->read_block(1, tempBuffer);
	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		if (tempBuffer[i] == 0){
			for (int j = 7; j < 64; j++){
				iosystem->read_block(j, tempBuffer);
				if (tempBuffer[0] == 0){
					for (int k = 0; k < 10; k++){
						if (symbolic_file_name[k] == 0){
							break;
						}
						tempBuffer[k] = symbolic_file_name[k];
						tempBuffer[10] = fileDescriptorIndex;
					}
					
					iosystem->write_block(j, tempBuffer);
					iosystem->read_block(1, tempBuffer);
					tempBuffer[0] += 11;
					tempBuffer[i] = j;
					iosystem->write_block(1, tempBuffer);
					iosystem->read_block((fileDescriptorIndex % 6) + 1, tempBuffer);
					int tempPos = fileDescriptorIndex / 6 * 4;
					tempBuffer[tempPos] = 1;
					iosystem->write_block((fileDescriptorIndex % 6) + 1, tempBuffer);
					iosystem->read_block(0, tempBuffer);
					tempBuffer[(fileDescriptorIndex % 6) + 1] = 1;
					tempBuffer[j] = 1;
					iosystem->write_block(0, tempBuffer);
					std::cout << "File creation success at new block." << std::endl;
					return fileDescriptorIndex;
				}
			}
		}
		else{
			int tempIndex = tempBuffer[i];
			iosystem->read_block(tempBuffer[i], tempBuffer);
			for (int j = 0; j < 64; j += 11){
				if (tempBuffer[j] == 0){
					if (j + 11 >= 64){
						break;
					}
					for (int k = 0; k < 10; k++){
						if (symbolic_file_name[k] == 0){
							break;
						}
						tempBuffer[j+k] = symbolic_file_name[k];
					}
					tempBuffer[j + 10] = fileDescriptorIndex;
					iosystem->write_block(tempIndex, tempBuffer);
					iosystem->read_block(1, tempBuffer);
					tempBuffer[0] += 11;
					iosystem->write_block(1, tempBuffer);
					iosystem->read_block((fileDescriptorIndex % 6) + 1, tempBuffer);
					int tempPos = fileDescriptorIndex / 6 * 4;
					tempBuffer[tempPos] = 1;
					iosystem->write_block((fileDescriptorIndex % 6) + 1, tempBuffer);
					iosystem->read_block(0, tempBuffer);
					tempBuffer[(fileDescriptorIndex % 6) + 1] = 1;
					iosystem->write_block(0, tempBuffer);
					std::cout << "File creation success at old block." << std::endl;
					return fileDescriptorIndex;
				}
			}
		}
	}
	return 0;
}

int FileSystem::deleteFile(std::string fileName){
	//int descriptorIndex = -1;

	//for (int i = 0; i < 14 && descriptorIndex == -1; ++i){
	//	dirEntry currentDirEntry = dir[i];
	//	if(currentDirEntry.symbolic_file_name.compare(fileName) == 0)
	//	{
	//		descriptorIndex = currentDirEntry.descriptorIndex;
	//	}
	//}
	//
	//if (descriptorIndex == -1){
	//	std::cout << "No file of name " << fileName << " found." << std::endl;
	//	return -1;
	//}
	////update bit map

	////iosystem->freeFileDescriptor(descriptorIndex);
	//dir[descriptorIndex] = dirEntry();
	//std::cout << "File of name " << fileName << " at position " << descriptorIndex << " was deleted." << std::endl;

	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		if (tempBuffer[i] == 0){
			continue;
		}
		int temp = tempBuffer[i];
		iosystem->read_block(tempBuffer[i], tempBuffer);
		for (int j = 0; j < 64; j += 11){
			bool match = true;
			for (int k = 0; fileName[k] != 0; k++){
				if (k == 10){
					break;
				}
				if (tempBuffer[j + k] != fileName[k]){
					match = false;
					break;
				}
			}
			if (match){
				for (int k = 0; k < 10; k++){
					tempBuffer[j + k] = 0;
				}
				char temp2 = tempBuffer[j + 10];
				tempBuffer[j + 10] = 0;
				iosystem->write_block(temp, tempBuffer);
				iosystem->read_block((temp2 % 6) + 1, tempBuffer);
				int tempPosition = temp2 / 6 * 4;
				tempBuffer[tempPosition] = 0;
				int blocksToClear[3];
				for (int no = 1; no < 4; no++){
					blocksToClear[no - 1] = tempBuffer[tempPosition + no];
					tempBuffer[tempPosition + no] = 0;
				}
				iosystem->write_block((temp2 % 6) + 1, tempBuffer);
				for (int no = 0; no < 3; no++){
					if (blocksToClear[no] == 0){
						break;
					}
					iosystem->read_block(blocksToClear[no], tempBuffer);
					for (int plz = 0; plz < 64; plz++){
						tempBuffer[plz] = 0;
					}
					iosystem->write_block(blocksToClear[no], tempBuffer);
				}
				iosystem->read_block(1, tempBuffer);
				tempBuffer[0] -= 11;
				iosystem->write_block(1, tempBuffer);
				return 1;
			}
		}

	}

	return -1;
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
