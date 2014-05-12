#include <string>

#ifndef SHELL_HPP
#define SHELL_HPP

class Shell
{
	private:
/*		FileSystem* filesystem; */
	
	public:
/* 		Shell(FileSystem* filesystem); */
		Shell();
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
