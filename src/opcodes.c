#include "chip.h"
#include "cpu_helpers.h"


void storeNumber(chip8 *c8, uint16_t code){  //For opcode 6XNN where x is the register and NN is the value
	uint8_t value = (code & 0xFF);
	uint8_t reg = (code >> 8) & 0xF;

	c8 -> dataRegister[reg] = value;
	pcIncr(c8);
}

void storeRegValue(chip8 *c8, uint16_t code){	//For opcode 8XY0 where x is the register where the value of y will be stored
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;

	uint8_t value = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = value;
	pcIncr(c8);

}

void addNumber(chip8 *c8, uint16_t code){	//For opcode 7XNN where X is the register and NN is the value to be added to the value at X
	uint8_t regX = (code >> 8) & 0xF;
	
	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t nValue = code & 0xFF;
	
	uint8_t sum;

	sum = (xValue + nValue) % 255;			//mod 255 to keep sum within 8 bits

	c8 -> dataRegister[regX] = sum;
	pcIncr(c8);


}


void addRegister(chip8 *c8, uint16_t code){	//For opcode 8XY4, add the value at register Y to the value at register x
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;
	
	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t yValue = c8 -> dataRegister[regY];
	
	uint8_t sum;
	
	if((xValue + yValue) > 255){				// If there will be an overflow(i.e. sum > 255), then F register will have 01, else F register will have 00
		c8 -> dataRegister[0xF] = 0x01;
	}
	else{
		c8 -> dataRegister[0xF] = 0x00;
	}
	sum = (xValue + yValue) % 255;			//mod 255 to keep sum within 8 bits

	c8 -> dataRegister[regX] = sum;
	pcIncr(c8);


}	

void subtract(chip8 *c8, uint16_t code){ //For opcode 8XY5 and opcode 8XY7. Both deal with subtraction and store at X register
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;
	
	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t yValue = c8 -> dataRegister[regY];

	uint8_t result;

	if((code & 0xF) == 0x5){				//opcode 8XY5
		if((xValue - yValue) < 0){
			c8 -> dataRegister[0xF] = 0x00;
		}
		else{
			c8 -> dataRegister[0xF] = 0x01;
		}
		result = abs(xValue - yValue);
	}
	else if((code & 0xF) == 0x7){				//opcode 8XY7
		if((yValue - xValue) < 0){
			c8 -> dataRegister[0xF] = 0x00;		//if negative set F to 00
		}
		else{
			c8 -> dataRegister[0xF] = 0x01;		//else set F to 01
		}
		result = abs(yValue - xValue);			//always return absolute value of subtraction
	}
	
	c8 -> dataRegister[regX] = result;
	pcIncr(c8);

}

void logicalAND(chip8 *c8, uint16_t code){	//opcode 8XY2. Set register VX to X AND Y.
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xf;

	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = xValue & yValue;
	pcIncr(c8);

}

void logicalOR(chip8 *c8, uint16_t code){	//opcode 8XY1. Set register VX to X OR Y.
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xf;

	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = xValue | yValue;
	pcIncr(c8);

}

void logicalXOR(chip8 *c8, uint16_t code){	//opcode 8XY3. Set register VX to X XOR Y.
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xf;

	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = xValue ^ yValue;
	pcIncr(c8);

}

void rightShift(chip8 *c8, uint16_t code){	//opcode 8XY6. Shift value in VY right one bit and store it in VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;

	uint8_t yValue = c8 -> dataRegister[regY];
	uint8_t xValue = c8 -> dataRegister[regX];
	
	c8 -> dataRegister[0xF] = xValue & 0x1;		//store least significant bit in VF
	c8 -> dataRegister[regX] = yValue >> 1;		//store the VY shifter to the right one bit in VX
	pcIncr(c8);

}

void leftShift(chip8 *c8, uint16_t code){		//opcode 8XYE. Shift value in VY left one bit and store it in VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;
	printf("%x\n", regY);


	uint8_t yValue = c8 -> dataRegister[regY];
	uint8_t xValue = c8 -> dataRegister[regX];

	c8 -> dataRegister[0xF] = (xValue >> 7) & 0x1;		//store the most significant bit in VF.
        c8 -> dataRegister[regX] = yValue << 1;	
	pcIncr(c8);

}

void randomNum(chip8 *c8, uint16_t code){			//CXNN. Stores a random number in VX that is between 0x00 and 0xFF and is masked with NN
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t mask = code & 0xFF;
	
	srand(time(NULL));
	uint8_t r = rand() % 255;

	r = r & mask;

	c8 -> dataRegister[regX] = r;
	pcIncr(c8);

}

void jumpAddress(chip8 *c8, uint16_t code){		//For Codes 1NNN and BNNN, where 1NNN jumps to address NNN and BNNN jumps to address NNN + V0
	uint8_t type = (code >> 12) & 0xF;		//To determine if 1 or B	
	uint16_t addr = (code & 0xFFF);
	uint8_t vValue = c8 -> dataRegister[0x0];

	if(type == 0x1){				//1NNN
		if(addr >= 0x200 && addr < 4096){	//make sure memory is inbounds and not accessing memory reserved for the interpreter.
			c8 -> programCounter = addr;	
		}
		else{
			exit(1);			//we will have to abort somehow. The program will probably crash too.
		}
	}
	else if (type == 0xB){				//BNNN
		if((addr + vValue) >= 0x200 && (addr + vValue) < 4096){
			c8 -> programCounter = addr + vValue;
		}
		else{
			exit(1);
		}

	}
}

void goToSubroutine(chip8 *c8, uint16_t code){	//For opcode 2NNN: execute subroutine starting at NNN	
	uint16_t returnAddr = c8 -> programCounter;		//this is to avoid an infinite loop. Note: i might change this depending the function to exit a subroutine
	c8 -> stack = push(c8 -> stack, returnAddr);

	uint16_t addr = (code & 0xFFF);
//	printf("subroutine at%x\n", addr);
	c8 -> programCounter = addr;
}

void returnSubroutine(chip8 *c8, uint16_t code){	//For opcode 00EE: return from subroutine
	uint16_t returnAddr = peek(c8 -> stack);
	c8 -> programCounter = returnAddr;

	c8 -> stack = pop(c8 -> stack);			//pop this address from the stack
	pcIncr(c8);   

}

void skipEqual(chip8 *c8, uint16_t code){	//3XNN skip the following instruction if the value of VX equal NN
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];

	uint8_t nValue = code & 0xFF;

	if(xValue == nValue){
		pcIncr(c8);
	}
	
	pcIncr(c8);
}

void skipNotEqual(chip8 *c8, uint16_t code){	//4XNN skip the following instruction if the value of VX is not equal to NN
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];

	uint8_t nValue = code & 0xFF;

	if(xValue != nValue){
		pcIncr(c8);
	}
	
	pcIncr(c8);
}

void skipRegEqual(chip8 *c8, uint16_t code){	//5XY0 skip the following instruction if the value of VX equals VY
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;

	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t yValue = c8 -> dataRegister[regY];

	if(xValue == yValue){
		pcIncr(c8);
	}
	
	pcIncr(c8);
}

void skipRegNotEqual(chip8 *c8, uint16_t code){	//9XY0 skip the following instruction if the value of VX does not equal VY
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;

	uint8_t xValue = c8 -> dataRegister[regX];
	uint8_t yValue = c8 -> dataRegister[regY];

	if(xValue != yValue){
		pcIncr(c8);
	}
	
	pcIncr(c8);
}

void setTimer(chip8 *c8, uint16_t code){		//FX15 set the delay timer to the value of register VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];

	c8 -> delayTimer = xValue;	
	pcIncr(c8);
}

void storeTimer(chip8 *c8, uint16_t code){		//FX07 store the value of delay timer in register VX.
	uint8_t regX = (code >> 8) & 0xF;

	c8 -> dataRegister[regX] = c8 -> delayTimer;
	pcIncr(c8);
}

void setSoundTimer(chip8 *c8, uint16_t code){		//FX18 set the sound timer to the value of register VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];

	c8 -> soundTimer = xValue;	
	pcIncr(c8);
}

void waitKeypress(chip8 *c8, uint16_t code){		//FX0A wait for a key press and store the result in register VX
//	SDL_Event keypress;
	uint8_t regX = (code >> 8) & 0xF;
	const Uint8 *states = SDL_GetKeyboardState( NULL );

	uint8_t pressed = 0;
	while(!pressed){
		if(states[SDL_SCANCODE_1]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x01;
		}
		else if(states[SDL_SCANCODE_2]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x02;
		}

		else if(states[SDL_SCANCODE_3]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x03;
		}
		else if(states[SDL_SCANCODE_4]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x0C;
		}
		else if(states[SDL_SCANCODE_Q]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x04;
		}
		else if(states[SDL_SCANCODE_W]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x05;
		}
		else if(states[SDL_SCANCODE_E]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x06;
		}
		else if(states[SDL_SCANCODE_R]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x0D;
		}
		else if(states[SDL_SCANCODE_A]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x07;
		}
		else if(states[SDL_SCANCODE_S]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x08;
		}
		else if(states[SDL_SCANCODE_D]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x09;
		}
		else if(states[SDL_SCANCODE_F]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x0E;
		}
		else if(states[SDL_SCANCODE_Z]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x0A;
		}
		else if(states[SDL_SCANCODE_X]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x00;
		}
		else if(states[SDL_SCANCODE_C]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x0B;
		}
		else if(states[SDL_SCANCODE_V]){
			pressed = 1;
			c8 -> dataRegister[regX] = 0x0F;
		}
	}

	pcIncr(c8);
}
	

void skipPress(chip8 *c8, uint16_t code){	//EX9E skip the following intruction if the key pressed is equal to the value of VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];
	const Uint8 *states = SDL_GetKeyboardState( NULL );

	if(states[RegToScancode(xValue)]){
		pcIncr(c8);
	}

	
	pcIncr(c8);
}

void skipNotPress(chip8 *c8, uint16_t code, uint8_t key){	//EXA1 skip the following intruction if the key pressed is not equal to the value of VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];
	const Uint8 *states = SDL_GetKeyboardState( NULL );

	if(!states[RegToScancode(xValue)]){
		pcIncr(c8);
	}
	
	pcIncr(c8);
}

void storeMemoryAddress(chip8 *c8, uint16_t code){	//ANNN store memory address NNN in the I register.
	uint16_t memAddr = (code & 0xFFF);
	c8 -> iRegister = memAddr;

	pcIncr(c8);
}

void addRegI(chip8 *c8, uint16_t code){		//FX1E add the value stored in register VX to register I
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];
	uint16_t iValue = c8 -> iRegister;
	uint16_t result;

	if((result = xValue + iValue) > MEM_SIZE){
		c8 -> dataRegister[0xF] = 1;
		result = result % MEM_SIZE + START_LOCATION;
	}
	else{
		c8 -> dataRegister[0xF] = 0;
	}


	c8 -> iRegister = result;
	pcIncr(c8);
	
}

void drawSprite(chip8 *c8, uint16_t code){  	//DXYN draw spite at the value stored in VX and VY starting with the memory address stored in I register. N bytes of data

	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;

	uint8_t xValue = c8 -> dataRegister[regX] % SCREEN_W;
	uint8_t yValue = c8 -> dataRegister[regY] % SCREEN_H;

	uint8_t nBytes = (code & 0xF);

	uint16_t addr = c8 -> iRegister;

	uint8_t i;
	uint8_t byteBits[9];		//8 bits + spot of '\0'

	for(i = 0 ; i < nBytes; i++){	
		getBits(byteBits, c8 -> dataMemory[addr + i]);
		writeBits(byteBits,xValue, yValue, c8);
		yValue++;
	}	
	setScreen(&(c8 -> chipScreen));
	SDL_RenderPresent(c8 -> chipScreen.renderer);
	pcIncr(c8);
}

void clearScreen(chip8 *c8, uint16_t code){		// opcode 00E0 clear screen of all pixel data

	memset(c8 -> chipScreen.binaryDisplay,BLACK ,SCREEN_W * SCREEN_H * sizeof(c8 -> chipScreen.binaryDisplay[0][0]));
	setScreen(&(c8 -> chipScreen));
	SDL_RenderPresent(c8 -> chipScreen.renderer);
	pcIncr(c8);
}

void setHex(chip8 *c8, uint16_t code){		//FX29 set i register to the hex value stored in register X.
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];

	if(xValue < 16){
		c8 -> iRegister = xValue * 5;
	}
	else{
		printf("attempt to access interpretrer memory\n");
	}
	pcIncr(c8);

}

void binDec(chip8 *c8, uint16_t code){		//FX33 set memory address I, I + 1, I + 2 to each of the decimal digits in the number stored at X.
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];
	uint16_t addr = c8 -> iRegister;


	uint8_t divisor = 100;
	int i;

	for(divisor, i = 0; i < 3; divisor /= 10, i++){
		c8 -> dataMemory[addr + i] = xValue / divisor;
		xValue = xValue % divisor;
	}
	pcIncr(c8);

}


void storei(chip8 *c8, uint16_t code){		//FX55 store values of V0 to VX inclusive starting at I in memory. Register I is set to I + X + 1 at the end.
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t startReg;
	uint16_t addr = c8 -> iRegister;

	for(startReg = 0x0; startReg <= regX; startReg++){
		c8 -> dataMemory[addr + startReg] = c8 -> dataRegister[startReg];
	}
	//c8 -> iRegister = addr + regX + 1;
	pcIncr(c8);
}

void loadi(chip8 *c8, uint16_t code){		//FX65 fill register V0 to VX with data starting at register I;
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t startReg;
	uint16_t addr = c8 -> iRegister;

	for(startReg = 0x0; startReg <= regX; startReg++){
		 c8 -> dataRegister[startReg] = c8 -> dataMemory[addr + startReg];
	}
	//c8 -> iRegister = addr + regX + 1;
	pcIncr(c8);

}



/*
int main(){

	chip8 c8;
	
	c8.programCounter = 0x20F;
	memset(c8.dataRegister, 0x00,REG_LOCATIONS * sizeof(char));
	c8.stack = NULL;
	chipInit(&c8);
	c8.dataRegister[0x0] = 0x11;
	c8.dataRegister[0xD] = 0x70;
	c8.dataRegister[0xA] = 0x70;

	SDL_Event event;
	int i = 10;	
	printf("HIT\n");
	while(i > 0){
		SDL_Delay(REFRESH_RATE);
		printf("SMALLHIT\n");
			
		i--;
	}	
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
}*/
