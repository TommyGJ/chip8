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
void printStack(struct linkedList *list){
	if(list != NULL){
		printf("stack info: %i %x\n", list -> location, list -> address);
		printStack(list -> next);
	}
}
void pcIncr(chip8 *c8){
	c8 -> programCounter += 2;
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

	uint8_t buffer;
	int i = START_LOCATION;
	
	while(fread(&buffer, sizeof(buffer), 1, rom)){
		c8 -> dataMemory[i] = buffer;
//		printf("%#04x at location %x\n", c8 -> dataMemory[i], i);
		i++;
	}
	c8 -> dataMemory[i] = '\0';
	fclose(rom);

}

uint16_t RegToScancode(uint8_t value){
	uint16_t scan;
	switch(value){
		case 0x01:
			scan = SDL_SCANCODE_1;
			break;
		case 0x02:
			scan = SDL_SCANCODE_2;
			break;
		case 0x03:
			scan = SDL_SCANCODE_3;
			break;
		case 0x04:
			scan = SDL_SCANCODE_Q;
			break;
		case 0x05:
			scan = SDL_SCANCODE_W;
			break;
		case 0x06:
			scan = SDL_SCANCODE_E;
			break;
		case 0x07:
			scan = SDL_SCANCODE_A;
			break;
		case 0x08:
			scan = SDL_SCANCODE_S;
			break;
		case 0x09:
			scan = SDL_SCANCODE_D;
			break;
		case 0x00:
			scan = SDL_SCANCODE_X;
			break;
		case 0x0A:
			scan = SDL_SCANCODE_Z;
			break;
		case 0x0B:
			scan = SDL_SCANCODE_C;
			break;
		case 0x0C:
			scan = SDL_SCANCODE_4;
			break;
		case 0x0D:
			scan = SDL_SCANCODE_R;
			break;
		case 0x0E:
			scan = SDL_SCANCODE_F;
			break;
		case 0x0F:
			scan = SDL_SCANCODE_V;
			break;
		default: 
			scan = SDL_SCANCODE_UNKNOWN;
			break;
	}
	return scan;
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
		if(c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_W] == bits[i]){
			c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_W] = BLACK;		//if both are white pixels or both are black pixel, the pixel will change to black: XOR mode
			if(bits[i] == WHITE){
				flag = 0x01;
			}
		}
		else{
			c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_W] = WHITE; 		//if there are different colors, then the pixel will change to WHITE
		}
	}	
	c8 -> dataRegister[0xF] = flag;
}

void loadSprites(uint8_t *sprites, uint8_t *memory){
	uint16_t start;

	for(start = 0x000; sprites[start] != '\0'; start++){
		memory[start] = sprites[start];
	}


}

void countTimer(chip8 *c8){		//for counting down the delay timer
	if(c8 -> delayTimer != 0x00){
		c8 -> delayTimer--;
	}
}

uint8_t find(uint16_t opcode){

	switch( (opcode >> 12) & 0xF){
		case 0x0:
			switch(opcode & 0xF){
				case 0x0:
					if(((opcode >> 4) & 0xF) == 0){
						return 0;
					}
					else{
						return 1;
					}
					break;
				case 0xE:
					return 2;
					break;
				default:
					break;
			}
			break;
		case 0x1:
			return 3;	
			break;
		case 0x2:
			return 4;
			break;
		case 0x3:
			return 5;	
			break;
		case 0x4:
			return 6;	
			break;
		case 0x5:
			return 7;
			break;
		case 0x6:
			return 8;
			break;
		case 0x7:
			return 9;	
			break;
		case 0x8:
			switch(opcode & 0xF){
				case 0x0:
					return 10;
					break;
				case 0x1:
					return 11;
					break;
				case 0x2:
					return 12;
					break;
				case 0x3:
					return 13;
					break;
				case 0x4:
					return 14;
					break;
				case 0x5:
					return 15;
					break;
				case 0x6:
					return 16;
					break;
				case 0x7:
					return 17;
					break;
				case 0xE:
					return 18;
					break;
				default:
					break;
			}

			break;
		case 0x9:
			return 19;
			break;
		case 0xA:
			return 20;	
			break;
		case 0xB:
			return 21;	
			break;
		case 0xC:
			return 22;	
			break;
		case 0xD:
			return 23;	
			break;
		case 0xE:
			switch(opcode & 0xF){
				case 0xE:
					return 24;	
					break;
				case 0x1:
					return 25;
					break;
				default:
					break;
			}
			break;
		case 0xF:
			switch((opcode >> 4) & 0xF){
				case 0x0:
					switch(opcode & 0xF){
						case 0x7:
							return 26;
							break;
						case 0xA:
							return 27;
							break;
						default:
							break;
					}
					break;
				case 0x1:
					switch(opcode & 0xF){
						case 0x5:
							return 28;
							break;
						case 0x8:
							return 29;
							break;
						case 0xE:
							return 30;
							break;
						default:
							break;
					}
					break;
				case 0x2:
					return 31;
					break;
				case 0x3:
					return 32;
					break;
				case 0x5:
					return 33;	
					break;
				case 0x6:
					return 34;
					break;

				default:
					break;
			}

			break;
		default:
			return 0;
			break;
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
