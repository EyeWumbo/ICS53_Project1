#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include "FileSystem.hpp"
#include "IOSystem.hpp"
#include "stdafx.h"

#ifndef SHELL_HPP
#define SHELL_HPP

class Shell
{
	private:
		FileSystem* filesystem; 
		char fileContentBuffer[192];
		void clearContent();
	
	public:
		Shell();
 		Shell(FileSystem* filesystem); 
		~Shell();
		void execute();
		int create(std::string name);
		int destroy(std::string name);
		int open(std::string name);
		void close(int index);
		int read(int index, int count);
		int write(int index, char value, int count);
		int seek(int index, int pos);
		void directory();
};

#endif
