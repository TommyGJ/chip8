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

void setScreen(screen *cs){
	SDL_Rect pixel;

	uint8_t xpos = 0;
	uint8_t ypos = 0;

	for(ypos = 0; ypos < SCREEN_H; ypos++){
		for(xpos = 0; xpos < SCREEN_W; xpos++){
			pixel.x = xpos * SCALE;
			pixel.y = ypos * SCALE;
			pixel.h = SCALE;
			pixel.w = SCALE;
			cs -> display[xpos][ypos] = pixel;
			if(cs -> binaryDisplay[xpos][ypos] == BLACK){
				SDL_SetRenderDrawColor(cs -> renderer, 0,0,0,255);
			}
			else{
				SDL_SetRenderDrawColor(cs -> renderer, 255,255,255,255);
			}	
			SDL_RenderFillRect(cs -> renderer,&(cs -> display[xpos][ypos])); 
		}
	}
}

void getBits(uint8_t *bits, uint8_t data){
	int i;
	int j;
	for(i = 7, j = 0; i >= 0; i--, j++){
		bits[i] = (data >> j) & 0x1;
	}
	bits[8] = '\0';
}

void writeBits(uint8_t *bits, uint8_t x, uint8_t y, chip8 *c8){
	int i;
	uint8_t flag = 0x00;
	for(i = 0; i < 8; i++){
		if(c8 -> chipScreen.binaryDisplay[x + i][y] == bits[i]){
			c8 -> chipScreen.binaryDisplay[x + i][y] = BLACK;		//if both are white pixels or both are black pixel, the pixel will change to black: XOR mode
			if(bits[i] == WHITE){
				flag = 0x01;
			}
		}
		else{
			c8 -> chipScreen.binaryDisplay[x + i][y] = WHITE; 		//if there are different colors, then the pixel will change to WHITE
		}
	}	
	c8 -> dataRegister[0xF] = flag;
}

void loadSprites(uint16_t *sprites, uint16_t *memory){
	uint16_t start;

	for(start = 0x000; sprites[start] != '\0'; start++){
		memory[start] = sprites[start];
	}


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
