#include <fstream>
#include <iostream>
#include <sstream>
#include "IOSystem.hpp"

using namespace std;

IOSystem::IOSystem(int l, int b)
{
	this->L = 64;
	this->B = 64;

	for (int i = 0; i < 64; i++){	
		for (int j = 0; j < 64; j++){
			ldisk[i][j] = 0;
		}
	}
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
		for (int i = 0; i < 64; i++){
			
			for (int j = 0; j < 64; j++){
				getline(saveFile, line, " "[0]);
				ldisk[i][j] = atoi(line.c_str());
			}
		}

		saveFile.close();
	}
	else{
		std::cout << "error" << std::endl;
	}
}

// Saves the array to a file as a disk image.
void IOSystem::save(){

	std::ofstream saveFile("saveFile.txt");

	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 64; j++){
			saveFile << (int)(ldisk[i][j]) << " ";
		}
		saveFile << std::endl;
	}

	saveFile.close();

}
