#ifndef IOSYSTEM_HPP
#define IOSYSTEM_HPP

#include <iostream>

class IOSystem
{
	private:
		int L;
		int B;
		bool bitmap[4][8];

		struct FileDescriptor{
			char symbolicName[10];
			unsigned char fileSize;
			unsigned char blockLocations[3];

			FileDescriptor(){
				for (int i = 0; i < 10; i++){
					symbolicName[i] = 0;
				}
				fileSize = 0;
				for (int i = 0; i < 3; i++){
					blockLocations[i] = 0;
				}
			}

			bool isEmpty(){
				return symbolicName[0] == 0;
			}

			friend std::ostream& operator <<(std::ostream& os, const FileDescriptor& fd){
				for (int i = 0; i < 10; i++){
					if (fd.symbolicName[i] == 0){
						break;
					}
					os << fd.symbolicName[i];
				}
				os << std::endl;
				os << (int)fd.fileSize << std::endl;
				for (int i = 0; i < 3; i++){
					os << (int)fd.blockLocations[i] << std::endl;
				}
				return os;
			}

		};

		struct Block{

			char memory[4];

			Block(){
				for (int i = 0; i < 4; i++){
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

		FileDescriptor cache[14];
		Block ldisk[64];

		Block currentBlock;
	
	public:
		IOSystem(int l, int b);
		~IOSystem();
		void read_block(int i, char *p);
		void write_block(int i, char *p);
		void save();
		void restore();
		int getNumDescriptors();
		int getNumBlocks();
};

#endif
