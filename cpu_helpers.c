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
		exit(0);
	}
	if(c8 -> programCounter >= 0xFFF){	//if an attempt is made to access memory not inbounds, quit.
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
