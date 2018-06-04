#include "chip.h"
#include "cpu_helpers.h"
#include "opcodes.h"


int main(int argc, char **argv){

	chip8 c8;
	
	chipInit(&c8);
	loadMemory(&c8, argv);
	

	SDL_Event event;

	while(c8.on){

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT: 
				      	chipQuit(&c8);
				      	exit(0);
				case SDL_KEYDOWN:
				       printf("%d\n", event.key.keysym.sym);
				       break;
			}
		}
		
	}
}
