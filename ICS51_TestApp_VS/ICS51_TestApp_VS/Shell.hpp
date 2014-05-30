#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include "FileSystem.hpp"
#include "IOSystem.hpp"

#ifndef SHELL_HPP
#define SHELL_HPP

class Shell
{
	private:
		FileSystem* filesystem;
		IOSystem* iosystem;
		char fileContentBuffer[192];
		void clearContent();
	
	public:
		Shell();
		~Shell();
		void execute();
		void create(std::string name);
		void destroy(std::string name);
		void open(std::string name);
		void close(int index);
		void read(int index, int count);
		void write(int index, char value, int count);
		void seek(int index, int pos);
		void directory();
		void save();
		void restore();
};

#endif
