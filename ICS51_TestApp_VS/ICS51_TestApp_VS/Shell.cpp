#include "Shell.hpp"

//
//Shell::Shell(FileSystem filesystem) {
//	this->filesystem = filesystem;
//}

Shell::Shell()
{
	filesystem = new FileSystem(new IOSystem(64, 64));
}

Shell::~Shell(){
	delete filesystem;
}

int Shell::create(std::string name){
 	return filesystem->create(name); 
	//return 1;
}

int Shell::destroy(std::string name){
 	return filesystem->deleteFile(name); 
	//return 1;
}

int Shell::open(std::string name){
	//NEED TO FIX OPEN METHOD TO RETURN INDEX
 	return filesystem->open(name); 
	//return 1;
}

void Shell::close(int index){
	//NEED TO FIX CLOSE TO RETURN SUCCESS/FAIL
 	filesystem->close(index); 
	return;
}

int Shell::read(int index, int count){
/*	char* mem_area; */
 	return filesystem->read(index, fileContentBuffer, count); 
	//return 1;
}

int Shell::write(int index, char value, int count){
 	return filesystem->write(index, value, count); 
	//return 1;
}

int Shell::seek(int index, int pos){
 	return filesystem->lseek(index, pos); 
	//return 1;
}

void Shell::directory(){
	filesystem->directory(); 
	//return;
}

/* Shell::initialize(){
} */

/* Shell::save(){
} */

void Shell::execute(){
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
				int success = create(name);
				if (success == -1)
					std::cout << "error" << std::endl;
				else
					std::cout << "file " << name << " created" << std::endl;
		}
		else if (command == "de"){
				std::string name;
				i >> name;
				int success = destroy(name);
				if (success == -1)
					std::cout << "error" << std::endl;
				else
					std::cout << "file " << name << " destroyed" << std::endl;
		}
		else if (command == "op"){
				std::string name;
				i >> name;
				int success = open(name);
				if (success == -1)
					std::cout << "error" << std::endl;
				else
					std::cout << "file " << name << " opened, index=" << success << std::endl;
		}
		else if (command == "cl"){
				int index;
				i >> index;
				close(index);
				std::cout << index << " closed" << std::endl;
		}
		else if (command == "rd"){
				int index, count;
				i >> index >> count;
				int success = read(index, count);
				if (success == -1)
					std::cout << "error" << std::endl;
				else
					std::cout << count << " bytes read: " << std::endl;
		}
		else if (command == "wr"){
				int index, count;
				char c;
				i >> index >> c >> count;
				int success = write(index, c, count);
				if (success == -1)
					std::cout << "error" << std::endl;
				else
					std::cout << count << " bytes written" << std::endl;
		}
		else if (command == "sk"){
				int index, pos;
				i >> index >> pos;
				int success = seek(index, pos);
				if (success == -1)
					std::cout << "error" << std::endl;
				else
					std::cout << "current position is " << pos << std::endl;
		}
		else if (command == "dr"){
				directory();
		}
		else if (command == "in"){
				std::cout << "disk initialized" << std::endl;
				/* std::cout << "disk restored" << std::endl; */
				/* initialize(); */
		}
		else if (command == "sv"){
				std::cout << "disk saved" << std::endl;
				/* save(); */
		}
		else if (command == "quit"){
				break;
		}
		else {
				std::cout << "error" << std::endl;
		}
	}
}

