#include "stdafx.h"
#include "FileSystem.hpp"
#include <iostream>
#include <string>

using namespace std;

FileSystem::FileSystem(IOSystem* iosystem){

	this->iosystem = iosystem;

	for(int i = 0; i < 3; i ++){
		openFileTable[i] = OFT();
	}
}

int FileSystem::open(std::string symbolicName){
	for (int i = 0; i < 14; i++){
		for (int j = 0; symbolicName[j] != 0; j++){
			if ()
		}
	}
	return -1;
}

