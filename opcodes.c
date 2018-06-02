#include "chip.h"
#include "stack.h"


void storeNumber(chip8 *c8, unsigned short code){  //For opcode 6XNN where x is the register and NN is the value
	unsigned char value = (code & 0xFF);
	unsigned char reg = (code >> 8) & 0xF;

	c8 -> dataRegister[reg] = value;
}

void storeRegValue(chip8 *c8, unsigned short code){	//For opcode 8XY0 where x is the register where the value of y will be stored
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xF;

	unsigned char value = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = value;
}

void addNumber(chip8 *c8, unsigned short code){	//For opcode 7XNN where X is the register and NN is the value to be added to the value at X
	unsigned char regX = (code >> 8) & 0xF;
	
	unsigned char xValue = c8 -> dataRegister[regX];
	unsigned char nValue = code & 0xFF;
	
	unsigned char sum;

	sum = (xValue + nValue) % 255;			//mod 255 to keep sum within 8 bits

	c8 -> dataRegister[regX] = sum;

}


void addRegister(chip8 *c8, unsigned short code){	//For opcode 8XY4, add the value at register Y to the value at register x
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xF;
	
	unsigned char xValue = c8 -> dataRegister[regX];
	unsigned char yValue = c8 -> dataRegister[regY];
	
	unsigned char sum;
	
	if((xValue + yValue) > 255){				// If there will be an overflow(i.e. sum > 255), then F register will have 01, else F register will have 00
		c8 -> dataRegister[0xF] = 0x01;
	}
	else{
		c8 -> dataRegister[0xF] = 0x00;
	}
	sum = (xValue + yValue) % 255;			//mod 255 to keep sum within 8 bits

	c8 -> dataRegister[regX] = sum;

}	

void subtract(chip8 *c8, unsigned short code){ //For opcode 8XY5 and opcode 8XY7. Both deal with subtraction and store at X register
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xF;
	
	unsigned char xValue = c8 -> dataRegister[regX];
	unsigned char yValue = c8 -> dataRegister[regY];

	unsigned char result;

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
}

void logicalAND(chip8 *c8, unsigned short code){	//opcode 8XY2. Set register VX to X AND Y.
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xf;

	unsigned char xValue = c8 -> dataRegister[regX];
	unsigned char yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = xValue & yValue;
}

void logicalOR(chip8 *c8, unsigned short code){	//opcode 8XY1. Set register VX to X OR Y.
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xf;

	unsigned char xValue = c8 -> dataRegister[regX];
	unsigned char yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = xValue | yValue;
}

void logicalXOR(chip8 *c8, unsigned short code){	//opcode 8XY3. Set register VX to X XOR Y.
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xf;

	unsigned char xValue = c8 -> dataRegister[regX];
	unsigned char yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[regX] = xValue ^ yValue;
}

void rightShift(chip8 *c8, unsigned short code){	//opcode 8XY6. Shift value in VY right one bit and store it in VX
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xF;

	unsigned char yValue = c8 -> dataRegister[regY];
	
	c8 -> dataRegister[0xF] = yValue & 0x1;		//store least significant bit in VF
	c8 -> dataRegister[regX] = yValue >> 1;		//store the VY shifter to the right one bit in VX
}

void leftShift(chip8 *c8, unsigned short code){		//opcode 8XY5. Shift value in VY left one bit and store it in VX
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char regY = (code >> 4) & 0xF;

	unsigned char yValue = c8 -> dataRegister[regY];

	c8 -> dataRegister[0xF] = (yValue >> 7) & 0x1;		//store the most significant bit in VF.
        c8 -> dataRegister[regX] = yValue << 1;	
}

void randomNum(chip8 *c8, unsigned short code){			//CXNN. Stores a random number in VX that is between 0x00 and 0xFF and is masked with NN
	unsigned char regX = (code >> 8) & 0xF;
	unsigned char mask = code & 0xFF;
	
	srand(time(NULL));
	unsigned char r = rand() % 255;

	r = r & mask;

	c8 -> dataRegister[regX] = r;
}

void jumpAddress(chip8 *c8, unsigned short code){		//For Codes 1NNN and BNNN, where 1NNN jumps to address NNN and BNNN jumps to address NNN + V0
	unsigned char type = (code >> 12) & 0xF;		//To determine if 1 or B	
	unsigned short addr = (code & 0xFFF);
	unsigned char vValue = c8 -> dataRegister[0x0];

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


int main(){

	chip8 c8;

	c8.programCounter = 0x200;
	memset(c8.dataRegister, 0x00,REG_LOCATIONS * sizeof(char));
	c8.stack = NULL;

	c8.dataRegister[0x0] = 0x11;
	c8.dataRegister[0xD] = 0x70;

	



}
