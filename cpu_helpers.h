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
#endif
