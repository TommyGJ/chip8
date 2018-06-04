#include "chip.h"

void chipInit(chip8 *c8){
	int errorCheck;
	errorCheck = SDL_Init(SDL_INIT_EVERYTHING);
	if(errorCheck != 0 ){
		printf("Unsuccessful initialization\n");
		exit(1);
	}
	else{
		printf("successful initialization\n");
	}
	
	c8 -> chipScreen.name = "chip8_chipScreen";
	c8 -> chipScreen.window = NULL;
	c8 -> chipScreen.renderer = NULL;
	c8 -> on = 1;

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
	printf("chip8 quit\n");
}





