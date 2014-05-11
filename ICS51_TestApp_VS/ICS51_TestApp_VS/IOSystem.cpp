#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "IOSystem.hpp"

using namespace std;

IOSystem::IOSystem(int l, int b)
{
	this->L = l;
	this->B = b;

	/*for (int i = 0; i < 4; i++){
		for (int j = 0; j < 7; j++){
			bitmap[i][j] = false;
		}
	}*/

	/*for (int i = 1; i < 14; i++){
		ldisk[i] = FileDescriptor();
	}*/

	for (int i = 0; i < 64; i++){	
		for (int j = 0; j < 64; j++){
			ldisk[i][j] = 0;
		}
	}
	

}

IOSystem::~IOSystem(){

}

void IOSystem::read_block(int i, char* p)
{	
	for (int b = 0; b < 64; b++)
	{
		p[b] = ldisk[i][b];
	}
}

void IOSystem::write_block(int i, char* p)
{	
	for (int b = 0; b < 64; b++)
	{
		ldisk[i][b] = p[b];
	}
	
}

void IOSystem::restore(){

	std::string line;
	std::ifstream saveFile("saveFile.txt");
	if (saveFile.is_open()){
		/*while (getline(saveFile, line)){
			std::stringstream ss;
			ss << line;
			int cacheNumber;
			ss >> cacheNumber;
			if (cacheNumber == -1){
				break;
			}
			getline(saveFile, line);
			cache[cacheNumber].fileSize = (char)atoi(line.c_str());
			for (int i = 0; i < 3; i++){
				getline(saveFile, line);
				cache[cacheNumber].blockLocations[i] = (char)atoi(line.c_str());
			}
			break;
		}


		while (getline(saveFile, line)){
			int blockNumber = atoi(line.c_str());
			getline(saveFile, line);
			for (int i = 0; line[i] != 0; i++){
				ldisk[blockNumber].memory[i] = line[i];
			}
		}*/

		for (int i = 0; i < 64; i++){
			getline(saveFile, line);
			for (int j = 0; j < 64; j++){
				ldisk[i][j] = line[j];
			}
		}

		saveFile.close();
	}
	else{
		std::cout << "It brokedededed" << std::endl;
	}

}

// Saves the array to a file as a disk image.
void IOSystem::save(){

	std::ofstream saveFile("saveFile.txt");
	/*for (int i = 0; i < 14; i++){
		if (cache[i].isOpen()){
			continue;
		}
		saveFile << i << std::endl;
		saveFile << cache[i];
	}

	saveFile << -1 << std::endl;
	for (int i = this->L; i < 64; i++){
		if (ldisk[i].isEmpty()){
			continue;
		}
		saveFile << i << std::endl;
		saveFile << ldisk[i];
	}*/

	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 64; j++){
			saveFile << ldisk[i][j];
		}
		saveFile << std::endl;
	}

	saveFile.close();

}
//
//int IOSystem::getNumBlocks(){
//	return this->B;
//}
//
//int IOSystem::getNumDescriptors(){
//	return this->L;
//}
//
//int IOSystem::findFreeDescriptor(){
//	for (int i = 0; i < 14; ++i){
//		if(cache[i].isOpen()){
//			cache[i].setAllocated();
//			return i;
//		}
//	}
//	return -1;
//}
//
//void IOSystem::freeFileDescriptor(int index){
//	cache[index].initialize();
//}