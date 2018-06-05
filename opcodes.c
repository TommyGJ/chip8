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
	
	c8 -> dataRegister[0xF] = yValue & 0x1;		//store least significant bit in VF
	c8 -> dataRegister[regX] = yValue >> 1;		//store the VY shifter to the right one bit in VX
	pcIncr(c8);

}

void leftShift(chip8 *c8, uint16_t code){		//opcode 8XY5. Shift value in VY left one bit and store it in VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t regY = (code >> 4) & 0xF;

	uint8_t yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[0xF] = (yValue >> 7) & 0x1;		//store the most significant bit in VF.
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
	c8 -> programCounter = addr;
}

void returnSubroutine(chip8 *c8, uint16_t code){	//For opcode 00EE: return from subroutine
	uint16_t returnAddr = c8 -> stack -> address;
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
	SDL_Event keypress;
	uint8_t regX = (code >> 8) & 0xF;

	int listen = 1;
	while(listen){
		while(SDL_PollEvent(&keypress)){
			switch(keypress.type){
				case SDL_KEYDOWN:
					if (goodkey(keypress.key.keysym.sym)){
						listen = 0;
						c8 -> dataRegister[regX] = determineKey(keypress.key.keysym.sym);
					}
					break;
				case SDL_QUIT:
					chipQuit(c8);
					exit(0);
					break;
				default:
					break;
			}
		}
	}
	pcIncr(c8);
}

void skipPress(chip8 *c8, uint16_t code, uint8_t key){	//EX9E skip the following intruction if the key pressed is equal to the value of VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];

	if(key == xValue){
		pcIncr(c8);
	}

	
	pcIncr(c8);
}

void skipNotPress(chip8 *c8, uint16_t code, uint8_t key){	//EXA1 skip the following intruction if the key pressed is not equal to the value of VX
	uint8_t regX = (code >> 8) & 0xF;
	uint8_t xValue = c8 -> dataRegister[regX];

	if(key != xValue){
		pcIncr(c8);
	}

	
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
