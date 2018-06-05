#include "chip.h"
#include "cpu_helpers.h"
#include "opcodes.h"


int main(int argc, char **argv){

	chip8 c8;
	
	chipInit(&c8);
	loadMemory(&c8, argv);
	c8.dataRegister[0xB] = 0x1;
	

	SDL_Event event;

	while(c8.on){
		uint8_t user_entry = 0xFF;		//garbage value that no user can enter
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT: 
				      	chipQuit(&c8);
				      	exit(0);
				case SDL_KEYDOWN:
					if(goodkey(event.key.keysym.sym)){
						user_entry = determineKey(event.key.keysym.sym);
					}
					break;
			}
		}
//		chipQuit(&c8);
		
		
	}
}
