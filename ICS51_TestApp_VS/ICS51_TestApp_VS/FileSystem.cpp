#include "stdafx.h"
#include "FileSystem.hpp"
#include <iostream>
#include <string>

using namespace std;

FileSystem::FileSystem(IOSystem* iosystem){

	this->iosystem = iosystem;

	/*for(int i = 0; i < 14; i ++){
		dir[i] = dirEntry();
	}*/
	for(int i = 0; i < 3; i ++){
		openFileTable[i] = OFT();
	}
	
	iosystem->read_block(0, tempBuffer);
	tempBuffer[0] = 1;
	tempBuffer[1] = 1;
	iosystem->write_block(0, tempBuffer);
	iosystem->read_block(1, tempBuffer);

}

int FileSystem::open(std::string symbolicName){

	OFT* entry = 0;

	for (int i = 0; i < 3; i++){
		if (openFileTable[i].isEmpty()){
			entry = &openFileTable[i];
			break;
		}
	}

	if (entry == nullptr){
		std::cout << "No OFT slots available." << std::endl;
		return -1;
	}

	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		if (tempBuffer[i] == 0){
			std::cout << "File with name " << symbolicName << " does not exist in the file system." << std::endl;
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
				for (int eval = 0; eval < 3; eval++){
					if (openFileTable[eval].fileDescriptorIndex == tempBuffer[j + 10]){
						std::cout << "File with name " << symbolicName << " already opened within OFT at slot " << eval << "." << std::endl;
						return -1;
					}
				}
				entry->fileDescriptorIndex = tempBuffer[j + 10];
				entry->currentPosition = 0;
				int tempPosition = tempBuffer[j + 10] / 6 * 4;
				iosystem->read_block(tempBuffer[j + 10] % 6 + 1, tempBuffer);
				iosystem->read_block(tempBuffer[tempPosition + 1], entry->bufferReader);
				std::cout << "Opened file with name " << symbolicName << " to OFT slot " << entry - openFileTable << "." << std::endl;
				std::cout << entry->fileDescriptorIndex << std::endl;
				return entry->fileDescriptorIndex;
			}
		}

	}

	return -1;
} 

int FileSystem::open_desc(int file_no){

	if (file_no > 13){
		std::cout << "Argument passed to file descriptor may not exceed the number of files (14). Arg given was " << file_no << "." << std::endl;
		return -1;
	}

	OFT* entry = 0;

	for (int i = 0; i < 3; i++){
		if (openFileTable[i].isEmpty()){
			entry = &openFileTable[i];
		}
		else{
			if (openFileTable[i].fileDescriptorIndex == file_no){
				std::cout << "File with descriptor index " << file_no << " already occupies slot " << i << " of the OFT." << std::endl;
				return -1;
			}
		}
	}

	if (entry == nullptr){
		std::cout << "No open OFT slots found when attempting to open descriptor number " << file_no << "." << std::endl;
		return -1;
	}

	iosystem->read_block(file_no % 6 + 1, tempBuffer);
	iosystem->read_block(tempBuffer[1], entry->bufferReader);
	entry->currentPosition = 0;;
	entry->fileDescriptorIndex = file_no;
	std::cout << "Successfully opened file descriptor of index " << file_no << " to OFT slot " << entry - openFileTable << "." << std::endl;
	return file_no;
}

void FileSystem::close(int index){

	OFT* entry = 0;

	for (int i = 0; i < 3; i++){
		if (openFileTable[i].fileDescriptorIndex == index){
			entry = &openFileTable[i];
		}
	}

	if (entry == 0){
		std::cout << "OFT slot with file descriptor index " << index << " not found." << std::endl;
		return;
	}

	if (entry->currentPosition == 0){
		entry->currentPosition = -1;
		entry->fileDescriptorIndex = -1;
		for (int i = 0; i < 64; i++){
			entry->bufferReader[i] = 0;
		}
		std::cout << "Empty file with descriptor index " << index << " closed. OFT slot " << entry - openFileTable << " freed." << std::endl;
		return;
	}

	int blockToWrite = entry->currentPosition / 64;
	if (blockToWrite == 3){
		blockToWrite--;
	}
	iosystem->read_block(index % 6 + 1, tempBuffer);
	iosystem->write_block(tempBuffer[blockToWrite + 1], entry->bufferReader);
	if (entry->currentPosition == 0){
		tempBuffer[index / 6 * 4] = 1;
	}
	else{
		tempBuffer[index / 6 * 4] = entry->currentPosition;
	}
	iosystem->write_block(index % 6 + 1, tempBuffer);
	entry->currentPosition = -1;
	entry->fileDescriptorIndex = -1;
	for (int i = 0; i < 64; i++){
		entry->bufferReader[i] = 0;
	}
	std::cout << "OFT slot " << entry - openFileTable << " cleared of file descriptor with index " << index << "." << std::endl;
}

void FileSystem::directory(){

	iosystem->read_block(1, tempBuffer);
	std::cout << "Root directory " << (int)(convertSize(tempBuffer[0])) << std::endl;
	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		iosystem->read_block(tempBuffer[i], tempBuffer);
		for (int j = 0; j < 53; j += 11){
			if (tempBuffer[j] == 0){
				continue;
			}
			for (int k = 0; k < 10; k++){
				if (k == 10 || tempBuffer[j + k] == 0){
					break;
				}
				std::cout << tempBuffer[j + k];
			}
			std::cout << " ";
			int tempIndex = tempBuffer[j+10];
			iosystem->read_block(tempIndex % 6 + 1, tempBuffer);
			std::cout << (int)(convertSize(tempBuffer[tempIndex / 6 * 4])) << std::endl;
			iosystem->read_block(1, tempBuffer);
			iosystem->read_block(tempBuffer[i], tempBuffer);
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
		int tempPosition = (i / 6) * 4;
		if (tempBuffer[tempPosition] == 0){
			fileDescriptorIndex = i;
			break;
		}
	}
	if (fileDescriptorIndex == 0){
		std::cout << "No files open" << std::endl;
		return -1;
	}
	int blockToWork = 0;
	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		if (tempBuffer[i] == 0){
			iosystem->read_block(0, tempBuffer);
			for (int j = 7; j < 64; j++){
				if (tempBuffer[j] == 0){
					blockToWork = j;
					iosystem->read_block(1, tempBuffer);
					tempBuffer[i] = blockToWork;
					iosystem->write_block(1, tempBuffer);
					break;
				}
			}
		}
		else{
			int tempIndex = tempBuffer[i];
			iosystem->read_block(tempBuffer[i], tempBuffer);
			for (int j = 0; j < 64; j += 11){
				if (j + 11 > 64){
					break;
				}
				if (tempBuffer[j] == 0){
					blockToWork = tempIndex;
					break;
				}
			}
		}
		if (blockToWork > 0){
			break;
		}
	}
	if (blockToWork == 0){
		return -1;
	}
	iosystem->read_block(blockToWork, tempBuffer);
	for (int i = 0; i < 64; i += 11){
		if (tempBuffer[i] == 0){
			for (int j = 0; symbolic_file_name[j] != 0; j++){
				if (j == 10){
					break;
				}
				tempBuffer[i + j] = symbolic_file_name[j];
			}
			tempBuffer[i + 10] = fileDescriptorIndex;
			iosystem->write_block(blockToWork, tempBuffer);
			iosystem->read_block((fileDescriptorIndex % 6) + 1, tempBuffer);
			int tempPos = fileDescriptorIndex / 6 * 4;
			tempBuffer[tempPos] = 1;
			iosystem->write_block((fileDescriptorIndex % 6) + 1, tempBuffer);
			iosystem->read_block(0, tempBuffer);
			tempBuffer[blockToWork] = 1;
			tempBuffer[(fileDescriptorIndex % 6) + 1] = 1;
			iosystem->write_block(0, tempBuffer);
			iosystem->read_block(1, tempBuffer);
			tempBuffer[0] += 11;
			iosystem->write_block(1, tempBuffer);
			break;
		}
	}
	return 0;
}

int FileSystem::deleteFile(std::string fileName){

	for (int i = 1; i < 4; i++){
		iosystem->read_block(1, tempBuffer);
		if (tempBuffer[i] == 0){
			continue;
		}
		int temp = tempBuffer[i];
		iosystem->read_block(tempBuffer[i], tempBuffer);
		for (int j = 0; j < 53; j += 11){
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
				std::cout << (int)(temp2) << std::endl;
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
					iosystem->read_block(0, tempBuffer);
					tempBuffer[blocksToClear[no]] = 0;
					iosystem->write_block(0, tempBuffer);
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
	OFT* entry = 0;

	for (int i = 0; i < 3; i++){
		if (openFileTable[i].fileDescriptorIndex == index){
			entry = &openFileTable[i];
		}
	}

	if (entry == nullptr){
		return -1;
	}
	iosystem->read_block(index % 6 + 1, tempBuffer);
	iosystem->read_block(tempBuffer[index / 6 * 4 + entry->currentPosition / 64 + 1], entry->bufferReader);
	for (int i = 0; i < count; i++){
		if (entry->currentPosition == 192){
			return -1;
		}
		if (entry->currentPosition % 64 == 0){
			int blockToRead = entry->currentPosition / 64;
			iosystem->read_block(index % 6 + 1, tempBuffer);
			if (blockToRead == 3){
				return -1;
			}
			iosystem->read_block(tempBuffer[(index / 6 * 4) + blockToRead + 1], entry->bufferReader);
		}
		//std::cout << entry->bufferReader[entry->currentPosition] << std::endl;
		mem_area[i] = entry->bufferReader[entry->currentPosition % 64];
		entry->currentPosition++;
		
	}
	return 1;
}

int FileSystem::write(int index, char value, int count)
{

	OFT* entry = 0;
	for (int i = 0; i < 3; i++){
		if (openFileTable[i].fileDescriptorIndex == index){
			entry = &openFileTable[i];
		}
	}

	if (entry == nullptr){
		std::cout << "Index " << index << " not found in the OFT." << std::endl;
		return -1;
	}

	if (entry->currentPosition + count > 192){
		std::cout << "Cannot write " << count << " bytes to file with index " << index << " at buffer position " << entry->currentPosition << "." << std::endl;
		return -1;
	}

	iosystem->read_block(index % 6 + 1, tempBuffer);
	if (tempBuffer[index / 6 * 4 + (entry->currentPosition / 64 + 1)] == 0){
		iosystem->read_block(0, tempBuffer);
		for (int i = 7; i < 64; i++){			
			if (tempBuffer[i] == 0){
				tempBuffer[i] = 1;
				iosystem->write_block(0, tempBuffer);
				iosystem->read_block(index % 6 + 1, tempBuffer);
				tempBuffer[index / 6 * 4 + (entry->currentPosition / 64 + 1)] = i;
				iosystem->write_block(index % 6 + 1, tempBuffer);
				iosystem->read_block(i, tempBuffer);
				break;
			}
		}
	}
	for (int i = 0; i < count; i++){
		if (entry->currentPosition == 192){
			std::cout << "Cannot write, file overflow." << std::endl;
			return -1;
		}
		if (entry->currentPosition % 64 == 0 && entry->currentPosition != 0){
			int blockToWrite = entry->currentPosition / 64;
			int tempPosition = index / 6 * 4;
			iosystem->read_block(index % 6 + 1, tempBuffer);
			iosystem->write_block(tempBuffer[tempPosition + blockToWrite], entry->bufferReader);
			
			if (tempBuffer[tempPosition + blockToWrite + 1] == 0){
				iosystem->read_block(0, tempBuffer);
				for (int i = 7; i < 64; i++){
					if (tempBuffer[i] == 0){
						tempBuffer[i] = 1;
						iosystem->write_block(0, tempBuffer);
						iosystem->read_block(index % 6 + 1, tempBuffer);
						tempBuffer[tempPosition + blockToWrite + 1] = i;
						iosystem->write_block(index % 6 + 1, tempBuffer);
						break;
					}
				}
			}
			iosystem->read_block(tempBuffer[tempPosition + blockToWrite + 1], entry->bufferReader);
		}
		
		entry->bufferReader[entry->currentPosition % 64] = value;
		entry->currentPosition++;
	}

	
	int blockToWrite = entry->currentPosition / 64;
	if (blockToWrite == 3){
		blockToWrite--;
	}
	iosystem->read_block(index % 6 + 1, tempBuffer);
	tempBuffer[index / 6 * 4] = entry->currentPosition;
	iosystem->write_block(index % 6 + 1, tempBuffer);
	if (tempBuffer[blockToWrite + 1] == 0){
		iosystem->read_block(0, tempBuffer);
		for (int i = 7; i < 64; i++){
			if (tempBuffer[i] == 0){
				tempBuffer[i] = 1;
				iosystem->write_block(0, tempBuffer);
				iosystem->read_block(index % 6 + 1, tempBuffer);
				tempBuffer[blockToWrite + 1] = i;
				iosystem->write_block(index % 6 + 1, tempBuffer);
				break;
			}
		}	
	}
	iosystem->write_block(tempBuffer[index / 6 * 4 + blockToWrite + 1], entry->bufferReader);
	std::cout << "Wrote char: " << value << " " << count << " times to index " << index << "." << std::endl;
    return 1;
}

int FileSystem::lseek(int index, int pos)
{
	OFT* entry = 0;

	for (int i = 0; i < 3; i++){
		if (openFileTable[i].fileDescriptorIndex == index){
			entry = &openFileTable[i];
		}
	}

	if (entry == nullptr){
		return -1;
	}

	int blockNumber = pos / 64;
	if (blockNumber == 3){
		blockNumber--;
	}
	
	entry->currentPosition = pos;

    return 1;
}

unsigned char FileSystem::convertSize(const char arg){
	if (arg < 0){
		return 2 * 128 + arg;
	}
	return arg;
}
