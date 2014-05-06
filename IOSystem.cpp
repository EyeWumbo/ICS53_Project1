#include <fstream>
#include <iostream>
#include "IOSystem.hpp"

using namespace std;

IOSystem::IOSystem(int l, int b)
{
	this->L = l;
	this->B = b;
	ldisk = new char*[L];
	for (int i = 0; i < L; i++)
	{
		ldisk[i] = new char[B];
	}
	
	/*
	USED FOR TESTING
 	for (int i=0; i<L; i++)
	{
		char array [] = "abcde";
		for (int j=0; j<B; j++)
		{
			ldisk[i][j] = array[j];
		}
	}*/
}

IOSystem::~IOSystem()
{
	for(int i = 0; i < L; i++)
		delete[] ldisk[i];
	delete[] ldisk;
}

void IOSystem::read_block(int i, char* p)
{	
	for (int b = 0; b < B; b++)
	{
		char c = ldisk[i][b];
		p[b] = c;
	}
}

void IOSystem::write_block(int i, char* p)
{	
	for (int b = 0; b < B; b++)
	{
		char c = p[b];
		ldisk[i][b] = c;
	}
}

void restore(){

	string line;
	std::ifstream saveFile("saveFile.txt");
	if (saveFile.is_open()){
		while (getline(saveFile, line)){
			std::stringstream ss;
			ss << line;
			int blockNumber;
			std::string content;
			ss >> blockNumber;
			ss >> content;

			if (blockNumber < this->K){
				std::strcpy(file_desc[blockNumber], content.c_str());
			}
			else{
				std::strcpy(ldisk[blockNumber], content.c_str());
			}	
		}
		saveFile.close();
	}
	else{
		std::cout << "It brokedededed" << std::endl;
	}

}

// Saves the array to a file as a disk image.
void save(){

	std::ofstream saveFile("saveFile.txt");
	for (int i = 0; i < L; i++){
		if (file_desc[i] == nullptr){
			continue;
		}
		saveFile << i << " ";
		for (int j = 0; j < B; j++){
			saveFile << file_desc[i][j];
			if (file_desc[i][j] == 0){
				saveFile << std::endl;
				break;
			}
		}
	}
	for (int i = this->L; i < B; i++){
		if (ldisk[i] == nullptr){
			continue;
		}
		saveFile << i << " ";
		for (int j = 0; j < B; j++){
			saveFile << ldisk[i][j];
			if (ldisk[i][j] == 0){
				saveFile << std::endl;
				break;
			}
		}
	}
	saveFile.close();

}

int getNumBlocks(){
	return B;
}

int getNumDescriptors(){
	return L;
}