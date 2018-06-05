#include "chip.h"
void updateLocations(struct linkedList *, int);
struct linkedList *push(struct linkedList *list, uint16_t data){
	struct linkedList *newEntry;
	if((newEntry = (struct linkedList *)malloc(sizeof(struct linkedList))) == NULL){
		exit(0);
	}
	newEntry -> next = list;
	newEntry -> address = data;
	
	updateLocations(newEntry, 0);	
	return newEntry;	
}

void updateLocations(struct linkedList *list, int entry){
	if(entry == 16){
		printf("out of bounds");
		exit(0);
	}
	if(list != NULL){
		list -> location = entry;
		updateLocations(list -> next, entry + 1);
	}
}
struct linkedList *pop(struct linkedList *list){
	if(list == NULL){
		return NULL;
	}
	struct linkedList *newHead = list -> next;
	free(list);

	updateLocations(newHead, 0);
	return newHead;

}
void pcIncr(chip8 *c8){
	c8 -> programCounter++;
	if(c8 -> programCounter < 0x200){	//if an attempt is made to access memory not reserved for the interpreter, quit.
		printf("attempt to acces memory out of range\n");
		exit(0);
	}
	if(c8 -> programCounter >= 0xFFF){	//if an attempt is made to access memory not inbounds, quit.
		printf("exceeds memory\n");
		exit(0);
	}

}

void loadMemory(chip8 *c8, char **argv){
	
	FILE *rom;

	if((rom = fopen(argv[1], "rb")) == NULL){
		printf("Cant read file\n");
		exit(0);
	}

	uint16_t buffer;
	int i = START_LOCATION;
	
	while(fread(&buffer, sizeof(buffer), 1, rom)){
		c8 -> dataMemory[i] = ntohs(buffer);
//		printf("%#06x at location %x\n", c8 -> dataMemory[i], i);
		i++;
	}
	c8 -> dataMemory[i] = '\0';
	fclose(rom);

}

int goodkey(int keyInfo){
	int good = 0;
	switch(keyInfo){
		case '1':
			good = 1;
			break;
		case '2':
			good = 1;
			break;
		case '3':
			good = 1;
			break;
		case '4':
			good = 1;
			break;
		case 'q':
			good = 1;
			break;
		case 'w':
			good = 1;
			break;
		case 'e':
			good = 1;
			break;
		case 'r':
			good = 1;
			break;
		case 'a':
			good = 1;
			break;
		case 's':
			good = 1;
			break;
		case 'd':
			good = 1;
			break;
		case 'f':
			good = 1;
			break;
		case 'z':
			good = 1;
			break;
		case 'x':
			good = 1;
			break;
		case 'c':
			good = 1;
			break;
		case 'v':
			good = 1;
			break;
		default:
			good = 0;
			break;
	}
	return good;
}

uint8_t determineKey(int keyInfo){
	uint8_t key;
	switch(keyInfo){
		case '1':
			key = 0x01;
			break;
		case '2':
			key = 0x02;
			break;
		case '3':
			key = 0x03;
			break;
		case '4':
			key = 0x0c;
			break;
		case 'q':
			key = 0x04;
			break;
		case 'w':
			key = 0x05;
			break;
		case 'e':
			key = 0x06;
			break;
		case 'r':
			key = 0x0d;
			break;
		case 'a':
			key = 0x07;
			break;

		case 's':
			key = 0x08;
			break;
		case 'd':
			key = 0x09;
			break;
		case 'f':
			key = 0x0E;
			break;
		case 'z':
			key = 0x0A;
			break;
		case 'x':
			key = 0x00;
			break;
		case 'c':
			key = 0x0B;
			break;
		case 'v':
			key = 0x0F;
			break;
		default:
			break;
	}
	return key;
}
/*
int main(){
	chip8 c8;
	c8.stack = NULL;
	c8.stack = push(c8.stack, 0x210);
	c8.stack = push(c8.stack, 0x300);
	c8.stack = push(c8.stack, 0x500);

	printf("%x at %i and %x at %i\n", c8.stack -> address,c8.stack -> location, c8.stack -> next -> address, c8.stack -> next -> location);
	c8.stack = pop(c8.stack);
	printf("%x at %i and %x at %i\n", c8.stack -> address,c8.stack -> location, c8.stack -> next -> address, c8.stack -> next -> location);
}*/
