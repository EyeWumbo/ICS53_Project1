#ifndef IOSYSTEM_HPP
#define IOSYSTEM_HPP

#include <iostream>

class IOSystem
{
	private:
		int L;
		int B;
		//bool bitmap[4][8];

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

		//struct FileDescriptor : Block{
		//	unsigned char fileSizeOne;
		//	unsigned char blockLocationsOne[3];
		//	unsigned char fileSizeTwo;
		//	unsigned char blockLocationsTwo[3];
		//	bool isFree;

		//	FileDescriptor(){
		//		initialize();
		//	}

		//	//resets parameters
		//	void initialize()
		//	{
		//		fileSizeOne = 0;
		//		fileSizeTwo = 0;
		//		isFree = true;
		//		for (int i = 0; i < 3; ++i){
		//			blockLocationsOne[i] = 0;
		//			blockLocationsTwo[i] = 0;
		//		}
		//	}

		//	bool isOpen(){
		//		return isFree;
		//	}

		//	void setAllocated()
		//	{
		//		isFree = false;
		//	}

		//	friend std::ostream& operator <<(std::ostream& os, const FileDescriptor& fd){
		//		os << std::endl;
		//		os << (int)fd.fileSizeOne << std::endl;
		//		for (int i = 0; i < 3; i++){
		//			os << (int)fd.blockLocationsOne[i] << std::endl;
		//		}
		//		os << (int)fd.fileSizeTwo << std::endl;
		//		for (int i = 0; i < 3; i++){
		//			os << (int)fd.blockLocationsTwo[i] << std::endl;
		//		}
		//		return os;
		//	}

		//};

		//FileDescriptor cache[14];
		unsigned char ldisk[64][64];

		//Block currentBlock;
	
	public:
		IOSystem(int l, int b);
		~IOSystem();
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
