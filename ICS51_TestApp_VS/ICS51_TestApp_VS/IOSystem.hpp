//============================================================================
// Author      : Leonard Bejosano, Melody Truong, TC Nguyen, Steven Zhang
// Version     : 0.1.2.1.2a
// Copyright   : Your copyright notice
// Description : First Project Lab
//============================================================================

#ifndef IOSYSTEM_HPP
#define IOSYSTEM_HPP

#include <iostream>

class IOSystem
{
	private:
		int L;
		int B;
		bool bitmap[4][8];

		struct Block{

			unsigned char memory[64];

			Block(){
				for (int i = 0; i < 64; i++){
					memory[i] = 0;
				}
			}

			unsigned char operator [](const int index){
				return memory[index];
			}

			bool isEmpty(){
				return memory[0] == 0;
			}

			friend std::ostream& operator <<(std::ostream& os, const Block& block){
				for (int i = 0; i < 64; i++){
					if (block.memory[i] == 0){
						break;
					}
					os << block.memory[i];
				}
				return os;
			}

		};

		unsigned char ldisk[64][64];
	
	public:
		IOSystem(int l, int b);
		//~IOSystem();
		void read_block(int i, char *p);
		void write_block(int i, char *p);
		void save();
		void restore();
		int getNumDescriptors();
		int getNumBlocks();
		char* getCurrentBlock();
		int findFreeDescriptor();
		void freeFileDescriptor(int index);
};

#endif
