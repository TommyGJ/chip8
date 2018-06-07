#include "chip.h"
#include "cpu_helpers.h"


void storeNumber(chip8 *, uint16_t );  //For opcode 6XNN where x is the register and NN is the value

void storeRegValue(chip8 *, uint16_t );	//For opcode 8XY0 where x is the register where the value of y will be stored

void addNumber(chip8 *, uint16_t );	//For opcode 7XNN where X is the register and NN is the value to be added to the value at X

void addRegister(chip8 *, uint16_t );	//For opcode 8XY4, add the value at register Y to the value at register x

void subtract(chip8 *, uint16_t ); //For opcode 8XY5 and opcode 8XY7. Both deal with subtraction and store at X register

void logicalAND(chip8 *, uint16_t );	//opcode 8XY2. Set register VX to X AND Y.

void logicalOR(chip8 *, uint16_t );	//opcode 8XY1. Set register VX to X OR Y.

void logicalXOR(chip8 *, uint16_t );	//opcode 8XY3. Set register VX to X XOR Y.

void rightShift(chip8 *, uint16_t );	//opcode 8XY6. Shift value in VY right one bit and store it in VX

void leftShift(chip8 *, uint16_t );		//opcode 8XYE. Shift value in VY left one bit and store it in VX

void randomNum(chip8 *, uint16_t );			//CXNN. Stores a random number in VX that is between 0x00 and 0xFF and is masked with NN

void jumpAddress(chip8 *, uint16_t );		//For Codes 1NNN and BNNN, where 1NNN jumps to address NNN and BNNN jumps to address NNN + V0

void goToSubroutine(chip8 *, uint16_t );	//For opcode 2NNN: execute subroutine starting at NNN	

void returnSubroutine(chip8 *, uint16_t );	//For opcode 00EE: return from subroutine

void skipEqual(chip8 *, uint16_t );	//3XNN skip the following instruction if the value of VX equal NN

void skipNotEqual(chip8 *, uint16_t );	//4XNN skip the following instruction if the value of VX is not equal to NN

void skipRegEqual(chip8 *, uint16_t );	//5XY0 skip the following instruction if the value of VX equals VY

void skipRegNotEqual(chip8 *, uint16_t );	//9XY0 skip the following instruction if the value of VX does not equal VY

void setTimer(chip8 *, uint16_t );		//FX15 set the delay timer to the value of register VX

void storeTimer(chip8 *, uint16_t );		//FX07 store the value of delay timer in register VX.

void setSoundTimer(chip8 *, uint16_t );		//FX18 set the sound timer to the value of register VX

void waitKeypress(chip8 *, uint16_t);		//FX0A wait for a key press and store the result in register VX

void skipPress(chip8 *, uint16_t);	//EX9E skip the following intruction if the key pressed is equal to the value of VX

void skipNotPress(chip8 *, uint16_t);	//EXA1 skip the following intruction if the key pressed is not equal to the value of VX

void storeMemoryAddress(chip8 *, uint16_t);	//ANNN store memory address NNN in the I register.

void addRegI(chip8 *, uint16_t);		//FX1E add the value stored in register VX to register I

void drawSprite(chip8 *, uint16_t);  	//DXYN draw spite at the value stored in VX and VY starting with N bytes starting with the memory address stored in I register

void clearScreen(chip8 *, uint16_t);	// opcode 00E0 clear screen of all pixel data

void setHex(chip8 *, uint16_t);		//FX29 set i register to the hex value stored in register X.

void binDec(chip8 *, uint16_t);		//FX33 set memory address I, I + 1, I + 2 to each of the decimal digits in the number stored at X.

void storei(chip8 *, uint16_t);		//FX55 store values of V0 to VX inclusive starting at I in memory. Register I is set to I + X + 1 at the end.

void loadi(chip8 *, uint16_t);		//FX65 fill register V0 to VX with data starting at register I;
