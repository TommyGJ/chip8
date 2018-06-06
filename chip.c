#include "chip.h"
#include "cpu_helpers.h"

uint16_t builtInSprites[49] = { 0xF090, 0x9090, 0x00F0,	//0 starts at 0x000
		    0x2060, 0x2020, 0x0070,	//1 starts at 0x003
		    0xF010, 0xF080, 0x00F0,	//2 starts at 0x006
		    0xF010, 0xF010, 0x00F0,	//3 starts at 0x009
		    0x9090, 0xF010, 0x0010,	//4 starts at 0x00C
		    0xF080, 0xF010, 0x00F0,	//5 starts at 0x00F
		    0xF080, 0xF090, 0x00F0,	//6 starts at 0x012
		    0xF010, 0x2040, 0x0040,	//7 starts at 0x015
		    0xF090, 0xF090, 0x00F0,	//8 starts at 0x018
		    0xF090, 0xF010, 0x00F0,	//9 starts at 0x01B
		    0xF090, 0xF090, 0x0090,	//A starts at 0x01E
		    0xE090, 0xE090, 0x00E0,	//B starts at 0x021
		    0xF080, 0x8080, 0x00F0,	//C starts at 0x024
		    0xE090, 0x9090, 0x00E0,	//D starts at 0x027
		    0xF080, 0xF080, 0x00F0,	//E starts at 0x02A
		    0xF080, 0xF080, 0x0080,	//F starts at 0x02D
		    '\0'};

void chipInit(chip8 *c8){
	
	c8 -> programCounter = START_LOCATION;			//starting memory
	c8 -> iRegister = 0x0;
	c8 -> delayTimer = 0x0;
	c8 -> soundTimer = 0x0;
	c8 -> stack = NULL;
	memset(c8 -> dataRegister, 0x00, REG_LOCATIONS * sizeof(uint8_t));
	memset(c8 -> dataMemory, 0x000, MEM_SIZE * sizeof(uint16_t));
	memset(c8 -> chipScreen.binaryDisplay,BLACK ,SCREEN_W * SCREEN_H * sizeof(c8 -> chipScreen.binaryDisplay[0][0]));
	c8 -> on = 1;

	loadSprites(builtInSprites, c8 -> dataMemory);

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

	setScreen(&(c8 -> chipScreen));		//set screen to all black
	
	SDL_RenderPresent(c8 -> chipScreen.renderer);
	

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





