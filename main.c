#include "chip.h"
#include "cpu_helpers.h"
#include "opcodes.h"


int main(int argc, char **argv){

	chip8 c8;
	
	chipInit(&c8);
	loadMemory(&c8, argv);
	c8.dataRegister[0xB] = 0x3;
	c8.dataRegister[0xA] = 0xF;
	c8.dataRegister[0x0] = 0x1F;
	c8.dataRegister[0x1] = 0xF;

	c8.iRegister = 0x009;
	drawSprite(&c8, 0xDAB5);
	SDL_Delay(1000);	
	drawSprite(&c8, 0xD015);
	SDL_Delay(1000);	
	clearScreen(&c8, 0x00E0);


	SDL_Event event;
	uint8_t user_entry; 

	while(c8.on){
		user_entry = 0xFF;		//garbage value that no user can enter
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
				default:
					break;
			}	
		}  


		
//		chipQuit(&c8);
		
		
	}
}
