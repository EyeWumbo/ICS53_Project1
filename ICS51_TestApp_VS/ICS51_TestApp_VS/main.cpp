#include <iostream>
#include <string>
#include "IOSystem.hpp"
#include "FileSystem.hpp"
#include "Shell.hpp"

int main()
{
	Shell shell = Shell();
	shell.execute();
	
/* 	//Creation Test
	std::cout << "Creation test begin." << std::endl << std::endl;
	fSystem.create("hallo0");
	
	fSystem.create("hallo0");
	
	fSystem.create("hallo1");
	fSystem.create("hallo2");
	fSystem.create("hallo3");
	fSystem.create("hallo3");
	fSystem.create("hallo4");
	fSystem.create("hallo5");
	fSystem.create("hallo6");
	fSystem.create("hallo7");
	fSystem.create("hallo8");
	fSystem.create("hallo9");
	fSystem.create("hallo10");
	fSystem.create("hallo11");
	fSystem.create("hallo12");
	fSystem.create("hallo13");
	fSystem.create("hallo14");
	std::cout << std::endl;
	
	////Deletion Test
	std::cout << "Deletion test begin." << std::endl << std::endl;
	fSystem.deleteFile("hallo0");
	fSystem.create("hallo15");
	fSystem.deleteFile("hallo0");
	fSystem.deleteFile("hallo9");
	fSystem.create("hallo16");
	std::cout << std::endl;

	

	////Open Test
	std::cout << "Open test begin." << std::endl << std::endl;
	fSystem.open("hallo1");
	fSystem.open("hallo17");
	fSystem.open("hallo2");
	fSystem.open("hallo3");
	fSystem.open("hallo4");
	std::cout << std::endl;

	

	////Close Test
	std::cout << "Close test begin." << std::endl << std::endl;
	fSystem.close(1);
	fSystem.close(2);
	fSystem.close(3);
	fSystem.close(4);
	fSystem.close(5);
	std::cout << std::endl;

	

	////Open Description Files
	std::cout << "Open Description test begin." << std::endl << std::endl;
	fSystem.open_desc(2);
	fSystem.open_desc(21);
	fSystem.open_desc(3);
	fSystem.deleteFile("hallo4");
	fSystem.open_desc(4);
	fSystem.open_desc(5);
	std::cout << std::endl;

	//Write Testing
	fSystem.write(2, 'y', 10);
	fSystem.write(3, 'x', 64);
	std::cout << std::endl;

	fSystem.close(2);
	fSystem.close(3);
	fSystem.close(5);

	fSystem.open_desc(3);
	char info[10];
	fSystem.read(3, info, 10);

	for (int i = 0; i < 10; i++){
		std::cout << info[i];
	}

	std::cout << std::endl;

	fSystem.directory();

	system.save();
	system.restore();

	std::string temp;
	std::cin >> temp; */
    return 0;
}

