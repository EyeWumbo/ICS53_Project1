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

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 7; j++){
			bitmap[i][j] = false;
		}
	}

	for (int i = 0; i < 14; i++){
		cache[i] = FileDescriptor();
	}

	for (int i = 0; i < 64; i++){	
		ldisk[i] = Block();
	}

	std::string lel = "and so with the power of greyskull shit happened the end";
	for (int i = 0; i < 64; i++){
		if (lel[i] == '\0'){
			break;
		}
		ldisk[52].memory[i] = lel[i];
	}
	

}

IOSystem::~IOSystem(){

}

void IOSystem::read_block(int i, char* p)
{	
	for (int b = 0; b < B; b++)
	{
		Block c = ldisk[i];
		currentBlock = c;
	}
}

void IOSystem::write_block(int i, char* p)
{	
	/*for (int b = 0; b < B; b++)
	{
		char c = p[b];
		ldisk[i][b] = c;
	}*/
	ldisk[i] = currentBlock;
}

void IOSystem::restore(){

	std::string line;
	std::ifstream saveFile("saveFile.txt");
	if (saveFile.is_open()){
		while (getline(saveFile, line)){
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
		}
		saveFile.close();
		std::cout << ldisk[52] << std::endl;
		std::cin >> line;
	}
	else{
		std::cout << "It brokedededed" << std::endl;
	}

}

// Saves the array to a file as a disk image.
void IOSystem::save(){

	std::ofstream saveFile("saveFile.txt");
	for (int i = 0; i < 14; i++){
		if (cache[i].isEmpty()){
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
	}
	saveFile.close();

}

int IOSystem::getNumBlocks(){
	return this->B;
}

int IOSystem::getNumDescriptors(){
	return this->L;
}