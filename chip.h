#include <stdlib.h>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <math.h>
#include <time.h>
#include <string.h>

#ifndef CHIP_H			//header guards
#define CHIP_H

#define MEM_SIZE 4095		//size of memory in bytes on Chip 8.
#define START_LOCATION 512	//program data can start being stored here 0x200	
#define REG_LOCATIONS 16	//0 - F

struct linkedList{
		unsigned short address;
		struct linkedList *next;
		int location;
	};

typedef struct chip8{
	unsigned short dataMemory[MEM_SIZE];		//Eash instruction is 2 bytes 0x0000 to 0xFFFF
	unsigned char dataRegister[REG_LOCATIONS];	//Values in each reg are 1 byte. 0x00 to 0xFF
	unsigned short IRegister;			//16 bits
	unsigned short programCounter;			//2 bytes but only 12 bits will be used. 0x000 to 0xFFF b/c only 0xFFF memory locations. PC will probably start at 0x200.
	
	struct linkedList *stack; 

}chip8;

unsigned short opcode;






#endif
