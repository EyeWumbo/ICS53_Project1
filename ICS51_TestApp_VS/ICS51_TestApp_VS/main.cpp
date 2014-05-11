#include "stdafx.h"
#include <iostream>
#include <string>
#include "IOSystem.hpp"
#include "FileSystem.hpp"

int main()
{
	IOSystem system = IOSystem(10,5);
	system.save();
	system.restore();
	FileSystem fSystem = FileSystem(&system);
    return 0;
}

