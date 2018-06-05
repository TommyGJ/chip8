#include "chip.h"

void chipInit(chip8 *c8){
	
	c8 -> programCounter = START_LOCATION;			//starting memory
	c8 -> IRegister = 0x0;
	c8 -> delayTimer = 0x0;
	c8 -> soundTimer = 0x0;
	c8 -> stack = NULL;
	memset(c8 -> dataRegister, 0x00, REG_LOCATIONS * sizeof(uint8_t));
	memset(c8 -> dataMemory, 0x000, MEM_SIZE * sizeof(uint16_t));
	c8 -> on = 1;

	if((SDL_Init(SDL_INIT_EVERYTHING)) != 0 ){
		printf("Unsuccessful initialization\n");
		exit(1);
	}
	else{
		printf("successful initialization\n");
	}
	
	c8 -> chipScreen.name = "chip8_chipScreen";
	c8 -> chipScreen.window = NULL;
	c8 -> chipScreen.renderer = NULL;

	c8 -> chipScreen.window = SDL_CreateWindow(c8 -> chipScreen.name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_W * SCALE,
			SCREEN_H * SCALE,
			0);
	if(c8 -> chipScreen.window == NULL){
		printf("Can't create window\n");
	}

	c8 -> chipScreen.renderer = SDL_CreateRenderer(c8 -> chipScreen.window,
			-1, SDL_RENDERER_ACCELERATED);

	

	
}

void chipQuit(chip8 *c8){
	SDL_DestroyRenderer(c8 -> chipScreen.renderer);
	SDL_DestroyWindow(c8 -> chipScreen.window);
	c8 -> on = 0;
	c8 -> chipScreen.renderer = NULL;
	c8 -> chipScreen.window = NULL;
	SDL_Quit();
	exit(1);
	printf("chip8 quit\n");
}





