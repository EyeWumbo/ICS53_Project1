#include "Shell.hpp"

Shell::Shell(){
	iosystem = new IOSystem(64,64);
	filesystem = new FileSystem(iosystem);
}

Shell::~Shell(){
	delete filesystem;
}

void Shell::create(std::string name){
 	int success = filesystem->create(name);
	if (success == -1)
		std::cout << "error" << std::endl;
	else
		std::cout << "file " << name << " created" << std::endl;	
}

void Shell::destroy(std::string name){
 	int success = filesystem->deleteFile(name);
	if (success == -1)
		std::cout << "error" << std::endl;
	else
		std::cout << "file " << name << " deleted" << std::endl;
}

void Shell::open(std::string name){
 	int index = filesystem->open(name);
	if (index == -1)
		std::cout << "error" << std::endl;
	else
		std::cout << "file " << name << " opened, index=" << index << std::endl;
}

void Shell::close(int index){
 	filesystem->close(index);
	std::cout << "file with index " << index << " closed" << std::endl;
}

void Shell::read(int index, int count){
	for (int i=0; i<192; i++)
		fileContentBuffer[i] = 0;
 	int success = filesystem->read(index, fileContentBuffer, count);
	if (success == -1)
		std::cout << "error" << std::endl;
	else
		std::cout << count << " bytes read: " << fileContentBuffer << std::endl;
}

void Shell::write(int index, char value, int count){
 	int success = filesystem->write(index, value, count);
	if (success == -1)
		std::cout << "error" << std::endl;
	else
		std::cout << count << " bytes written" << std::endl;
}

void Shell::seek(int index, int pos){
 	int success = filesystem->lseek(index, pos);
	if (success == -1)
		std::cout << "error" << std::endl;
	else
		std::cout << "current position is " << pos << std::endl;
}

void Shell::directory(){
	filesystem->directory(); 
}

void Shell::save(){
	iosystem->save();
	std::cout << "disk saved" << std::endl;
}

void Shell::restore(){
	iosystem->restore();
	std::cout << "disk restored" << std::endl;
}

void Shell::execute(){
	bool initialized = false;
	while (true)
	{
        std::string input;
        getline(std::cin, input);
		std::istringstream i(input);
		std::string command;
		i >> command;
		if (command == "cr"){
				std::string name;
				i >> name;
				create(name);
		}
		else if (command == "de"){
				std::string name;
				i >> name;
				destroy(name);
		}
		else if (command == "op"){
				std::string name;
				i >> name;
				open(name);
		}
		else if (command == "cl"){
				int index;
				i >> index;
				close(index);
		}
		else if (command == "rd"){
				int index, count;
				i >> index >> count;
				read(index, count);
		}
		else if (command == "wr"){
				int index, count;
				char c;
				i >> index >> c >> count;
				write(index, c, count);
		}
		else if (command == "sk"){
				int index, pos;
				i >> index >> pos;
				seek(index, pos);
		}
		else if (command == "dr"){
				directory();
		}
		else if (command == "in"){
				if (!initialized){
					initialized = true;
					std::cout << "disk initialized" << std::endl;
				}
				else{
					restore();
				}
		}
		else if (command == "sv"){
				save();
		}
		else if (command == "q"){
				break;
		}
		else {
				std::cout << "error" << std::endl;
		}
	}
}

