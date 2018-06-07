#include "chip.h"

#ifndef CPU_HELPERS_H
#define CPU_HELPERS_H
void updateLocations(struct linkedList *, int);

struct linkedList *push(struct linkedList *, uint16_t);

struct linkedList *pop(struct linkedList *);

void pcIncr(chip8 *);

void loadMemory(chip8 *, char ** );

int goodkey(int);

uint8_t determineKey(int);

void setScreen(screen *);

void getBits(uint8_t *, uint8_t);

void writeBits(uint8_t *, uint8_t , uint8_t , chip8 *);

void loadSprites(uint8_t *, uint8_t *);

void countTimer(chip8 *);		//for counting down the delay timer

uint8_t find(uint16_t);
#endif
