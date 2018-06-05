#include <stdlib.h>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>


#ifndef CHIP_H			//header guards
#define CHIP_H

#define MEM_SIZE 4095		//size of memory in bytes on Chip 8.
#define START_LOCATION 512	//program data can start being stored here 0x200	
#define REG_LOCATIONS 16	//0 - F
#define REFRESH_RATE (unsigned int) 17	//same as 60hz
#define SCREEN_W 64
#define SCREEN_H 32
#define SCALE 20
#define SPRITE_W 8
#define MAX_SPRITE_H 15
#define WHITE 0x1
#define BLACK 0x0

uint16_t htons(uint16_t);
typedef struct screen{
	const char *name;
	SDL_Window *window;
	SDL_Renderer *renderer;

}screen;
struct linkedList{
		uint16_t address;
		struct linkedList *next;
		int location;
};

typedef struct chip8{
	uint16_t dataMemory[MEM_SIZE];		//Eash instruction is 2 bytes 0x0000 to 0xFFFF
	uint8_t dataRegister[REG_LOCATIONS];	//Values in each reg are 1 byte. 0x00 to 0xFF
	uint16_t iRegister;			//16 bits
	uint16_t programCounter;			//2 bytes but only 12 bits will be used. 0x000 to 0xFFF b/c only 0xFFF memory locations. PC will probably start at 0x200.
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t on;
		
	screen chipScreen;
	struct linkedList *stack; 

}chip8;

uint16_t opcode;

void chipInit(chip8 *);
void chipQuit(chip8 *);



#endif
