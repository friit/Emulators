/**
	CPU.cpp
	Purpose: CPU emulator for a Chip8 Processor

	@author Joseph Stevens
	@version 1 9/10/16
*/
#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include "CHIP8.h"
#pragma warning (disable : 4996)

void
CHIP8::run()
{

}

/**
Sets up CPU to be used
Clears the memory and register and sets the program counter to its start position

@param None
@return a bool on success
*/
bool 
CHIP8::initialize()
{
	//Clear memory to 0
	memset(memory, 0, 4096);
	memset(gfx, 0, 64 * 32);
	//memset(stack, 0, 16);
	memset(V, 0, 16);

	unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];


	//Program counter starts at 0x200
	pc = 0x200;

	sound_timer = 0;
	delay_timer = 0;

	I = 0;
	sp = 0;


	return false;
}
void 
CHIP8::KeyPressed(int i)
{
	key[i] = 1;
}


void 
CHIP8::KeyReleased(int i)
{
	key[i] = 0;
}

int
CHIP8::GetKeyPressed()
{
	int k = -1;
	for (int i = 0; i < 16; i++)
	{
		if (key[i] > 0)
			return i;
	}
}



/**
	Loads a Chip8 rom into memory

	@param char array of the filename
	@return bool on a success
*/
bool 
CHIP8::loadgame(std::string game)
{
	try {
		
		FILE *in;
		std::string filePath = "c:/CHIP8/" + game;
		const char* g = filePath.c_str();
		//strcat(filePath, game);
		in = fopen(g, "rb");
		fread(&memory[0x200], 0xFFF, 1, in);
		fclose(in);
	}
	catch (int e) {
		std::cout << "An exception occurred. Exception Nr. " << e << '\n';
		return false;
	}
	
	return true;
}

/**
	Emulates a single Chip8 CPU cycles
	Fetches, Decodes and Executes opcodes
	Draws game
	Handles Intterupts 
	Updates Timers

	@param None
	@return Void
*/
void 
CHIP8::emulateCycle()
{
	//Fetch opcode
	WORD Opcode = FetchNextOpcode();
	//Decode opcode
	DecodeOpcode(Opcode);


	// Update timers
	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
			//Beep();
		--sound_timer;
	}
}

/** 
	Fetches next OpCode from memory 
	Appends the next two bytes from memory into an opcode

	@param None
	@return the next opcode in memory
*/
WORD
CHIP8::FetchNextOpcode()
{
	//Complete opcode
	WORD op = 0;


	//Appends two bytes from memory together
	op = memory[pc];
	op <<= 8;
	op |= memory[pc + 1];

	//Updates the program counter
	pc += 2;

	return op;
}


/**
	Decodes and executes an opcode
	Decodes the first letter to give the opcode to the correct child function

	@param opcode stored in a WORD
	@return Void
*/
void
CHIP8::DecodeOpcode(WORD opcode)
{
	//& with 0xF00 to get the first number
 	WORD c = opcode & 0xF000;
 	switch (c)
	{
	case 0x0000: op0(opcode); break;
	case 0x1000: op1(opcode); break;
	case 0x2000: op2(opcode); break;
	case 0x3000: op3(opcode); break;
	case 0x4000: op4(opcode); break;
	case 0x5000: op5(opcode); break;
	case 0x6000: op6(opcode); break;
	case 0x7000: op7(opcode); break;
	case 0x8000: op8(opcode); break;
	case 0x9000: op9(opcode); break;
	case 0xA000: opA(opcode); break;
	case 0xB000: opB(opcode); break;
	case 0xC000: opC(opcode); break;
	case 0xD000: opD(opcode); break;
	case 0xE000: opE(opcode); break;
	case 0xF000: opF(opcode); break;

	default:
		break;
	}
}


/*
All opcodes that start with 0x0

0x0000 : CLS
	Clear Screen
	Resets graphics buffer

0x000E : RET
	Return from subroutine
	Sets the program counter to the adress at the top of the stack
	Then decrements stack pointer

0x0NNN : SYS addr
	Jump to a machine code routine at NNN
	This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.

*/
void 
CHIP8::op0(WORD opcode)
{
	switch (opcode & 0x00FF)
	{
	//CLS: 
	case 0x00E0:
		memset(gfx, 0, sizeof(gfx));
		break;
	//RET: 
	case 0x00EE:
		pc = Stack.back();
		Stack.pop_back();
		break;

	default:
		//SYS ADDR

		break;
	}
}

/*
All opcodes that start with 0x1

0x1NNN : JP addr
	Jump to location at NNN
	Sets the program counter to NNN

*/
void 
CHIP8::op1(WORD opcode)
{
	pc = opcode & 0x0FFF;
}

/*
All opcodes that start with 0x2

	0x2NNN : CALL addr
	Call subroutine at NNN
	Increments the stackpointer,  then puts the current program counter on the top of the stack
	program counter is then set to NNN

*/
void 
CHIP8::op2(WORD opcode)
{
	Stack.push_back(pc);
	pc = opcode & 0x0FFF;
}

/*
All opcodes that start with 0x3

	0x3XKK : SE VX, byte
	Skip next instruction if Vx = kk.
	The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.

*/
void
CHIP8::op3(WORD opcode)
{
	int kk = opcode & 0x00FF;
	int x = opcode & 0x0F00;
	x >>= 8;
	if (V[x] == kk) 
		pc += 2;
}


/*
All opcodes that start with 0x4

	0x4XKK : SNE Vx, byte
	Skip next instruction if VX != kk.
	The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.

*/
void
CHIP8::op4(WORD opcode)
{
	int kk = opcode & 0x00FF;
	int x = opcode & 0x0F00;
	x >>= 8;
	if (V[x] != kk)
		pc += 2;

}

/*
All opcodes that start with 0x5

	5xy0 : SE Vx, Vy
	Skip next instruction if Vx = Vy.
	The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.

*/
void 
CHIP8::op5(WORD opcode)
{
	int y = opcode & 0x00F0;
	int x = opcode & 0x0F00;
	x >>= 8;
	y >>= 4;
	if (V[x] == V[y])
		pc += 2;

}

/*
All opcodes that start with 0x6

	6xkk : LD Vx, byte
	Set Vx = kk.
	The interpreter puts the value kk into register Vx.

*/
void 
CHIP8::op6(WORD opcode)
{
	int kk = opcode & 0x00FF;
	int x = opcode & 0x0F00;
	x >>= 8;
	V[x] = kk;
}

/*
All opcodes that start with 0x7

	7xkk : ADD Vx, byte
	Set Vx = Vx + kk.
	Adds the value kk to the value of register Vx, then stores the result in Vx.

*/
void 
CHIP8::op7(WORD opcode)
{
	int kk = opcode & 0x00FF;
	int x = opcode & 0x0F00;
	x >>= 8;
	V[x] += kk;
}

/*
All opcodes that start with 0x8

	8xy0 : LD Vx, Vy
	Set Vx = Vy.
	Stores the value of register Vy in register Vx.


	8xy1 : OR Vx, Vy
	Set Vx = Vx OR Vy.
	Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.


	8xy2 : AND Vx, Vy
	Set Vx = Vx AND Vy.
	Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.


	8xy3 : XOR Vx, Vy
	Set Vx = Vx XOR Vy.
	Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.


	8xy4 : ADD Vx, Vy
	Set Vx = Vx + Vy, set VF = carry.
	The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.


	8xy5 : SUB Vx, Vy
	Set Vx = Vx - Vy, set VF = NOT borrow.
	If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.


	8xy6 : SHR Vx {, Vy}
	Set Vx = Vx SHR 1.
	If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.


	8xy7 : SUBN Vx, Vy
	Set Vx = Vy - Vx, set VF = NOT borrow.
	If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.


	8xyE : SHL Vx {, Vy}
	Set Vx = Vx SHL 1.
	If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.

*/
void 
CHIP8::op8(WORD opcode)
{
	//Extracts the xy values from 8xyN
	int x = opcode & 0x0F00; 
	x >>= 8;
	int y = opcode & 0x00F0; 
	y >>= 4;

	//Switches based on the final hex value
	switch (opcode & 0x000F)
	{
	case 0x0000: //8xy0
		V[x] = V[y];
		break;
	case 0x0001: //8xy1
		if (V[x] | V[y]) {
			V[x] = 1;
		}else{
			V[x] = 0;
		}
		break;
	case 0x0002:
		if (V[x] & V[y]) {
			V[x] = 1;
		}
		else {
			V[x] = 0;
		}
		break;
	case 0x0003:
		if (V[x] ^ V[y]) {
			V[x] = 1;
		}
		else {
			V[x] = 0;
		}
		break;
	case 0x0004: //ADD Vx,Vy
		if ((V[x] + V[y]) > 0x00FF) {
			V[15] = 1;

			V[x] = (V[x] + V[y]) & 0x00FF;
		}
		else {
			V[x] += V[y];
		}
		break;
	case 0x0005: //SUBB Vx,Vy
		if (V[x] > V[y]) {
			V[0x00F] = 1;

		}else{
			V[0x00F] = 0;

		}

		V[x] -= V[y];
		break;
	case 0x0006: //SHR Vx {,Vy}

		if (V[x] & 0x000F == 1) V[0x000F] = 1;
		else V[0x000F] = 0;
		V[x] = V[x] >> 1;

		break;
	case 0x0007: //SUBN
		if(V[y] <  V[x])
			V[0x000F] = 0;
		V[x] = V[y] - V[x];
		break;
	case 0x000E:
		if(V[x] & 0xF000 == 1) V[0x000F] = 1;
		else  V[0x000F] = 0;
		V[x] = V[x] << 1;
		break;
	default:
		break;
	}
}

/*
All opcodes that start with 0x9

	9xy0 : SNE Vx, Vy
	Skip next instruction if Vx != Vy.
	The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.

*/
void 
CHIP8::op9(WORD opcode)
{
	int x = opcode & 0x0F00;
	x >>= 8;
	int y = opcode & 0x00F0;
	y >>= 4;
	if (V[x] != V[y]) pc += 2;
}

/*
All opcodes that start with 0xA

	Annn : LD I, addr
	Set I = nnn.
	The value of register I is set to nnn.
*/
void 
CHIP8::opA(WORD opcode)
{
	I = opcode & 0x0FFF;
}

/*
All opcodes that start with 0xB

	Bnnn : JP V0, addr
	Jump to location nnn + V0.
	The program counter is set to nnn plus the value of V0.
*/
void 
CHIP8::opB(WORD opcode)
{
	pc = (opcode & 0x0FFF) + V[0];
}

/*
All opcodes that start with 0xC

	Cxkk : RND Vx, byte
	Set Vx = random byte AND kk.
	The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
*/
void 
CHIP8::opC(WORD opcode)
{
	int r = rand() % 255;
	V[(opcode & 0xF00) >> 8] = r & (opcode & 0x00FF);
}

/*
All opcodes that start with 0xD

Dxyn - DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
The interpreter reads n bytes from memory, starting at the address stored in I. 
These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. 
If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. 
See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
*/
void 
CHIP8::opD(WORD opcode)
{
	//int x = opcode & 0x0F00 >> 8;
	//int y = opcode & 0x00F0 >> 4;
	//int n = opcode & 0x000F;
	//int xcord = V[x];
	//int ycord = V[y];

	//V[0xF] = 0;

	//for (int yline = 0; yline < n; yline++) 
	//{
	//	BYTE data = memory[I + n];
	//	int xpixel = 0;

	//	for (xpixel = 0; xpixel < 8; xpixel++)
	//	{
	//		if (gfx[xcord + xpixel][ycord + n][0] == 1)
	//			V[0xF] = 1; //Collision
	//		gfx[xcord + xpixel][ycord + n][0] ^= 1;
	//	}


	//}
	const int SCALE = 10;
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;

	int coordx = V[regx] * SCALE;
	int coordy = V[regy] * SCALE;
	int height = opcode & 0x000F;
	draw = true;
	V[0xf] = 0;
	
	for (int yline = 0; yline < height; yline++)
	{
		// this is the data of the sprite stored at m_GameMemory[m_AddressI]
		// the data is stored as a line of bytes so each line is indexed by m_AddressI + yline
		BYTE data = (memory[I + yline]);

		// for each of the 8 pixels in the line
		int xpixel = 0;
		int xpixelinv = 7;
		for (xpixel = 0; xpixel < 8; xpixel++, xpixelinv--)
		{

			// is ths pixel set to 1? If so then the code needs to toggle its state
			int mask = 1 << xpixelinv;
			if (data & mask)
			{
				int x = (xpixel*SCALE) + coordx;
				int y = coordy + (yline*SCALE);

				int colour = 0;

				// a collision has been detected
				if (gfx[y][x][0] == 0)
				{
					colour = 255;
					V[15] = 1;
				}

				// colour the pixel
				for (int i = 0; i < SCALE; i++)
				{
					for (int j = 0; j < SCALE; j++)
					{
						gfx[y + i][x + j][0] = colour;
						gfx[y + i][x + j][1] = colour;
						gfx[y + i][x + j][2] = colour;
					}
				}

			}
		}
	}
}

/*
All opcodes that start with 0xE

	Ex9E : SKP Vx
	Skip next instruction if key with the value of Vx is pressed.
	Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.

	ExA1 - SKNP Vx
	Skip next instruction if key with the value of Vx is not pressed.
	Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.

*/
void 
CHIP8::opE(WORD opcode)
{
	switch (opcode & 0x00FF)
	{
	case 0x009E: if (key[V[(opcode & 0x0F00) >> 8]] == 1) 
		pc += 2;
		break;
	case 0x00A1: if (key[V[(opcode & 0x0F00) >> 8]] == 0) 
		pc += 2;
	default:
		break;
	}
}

/*
All opcodes that start with 0xF

	Fx07 : LD Vx, DT
	Set Vx = delay timer value.
	The value of DT is placed into Vx.


	Fx0A : LD Vx, K
	Wait for a key press, store the value of the key in Vx.
	All execution stops until a key is pressed, then the value of that key is stored in Vx.


	Fx15 : LD DT, Vx
	Set delay timer = Vx.
	DT is set equal to the value of Vx.


	Fx18 : LD ST, Vx
	Set sound timer = Vx.
	ST is set equal to the value of Vx.


	Fx1E : ADD I, Vx
	Set I = I + Vx.
	The values of I and Vx are added, and the results are stored in I.


	Fx29 : LD F, Vx
	Set I = location of sprite for digit Vx.
	The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.


	Fx33 : LD B, Vx
	Store BCD representation of Vx in memory locations I, I+1, and I+2.
	The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.


	Fx55 : LD [I], Vx
	Store registers V0 through Vx in memory starting at location I.
	The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.


	Fx65 : LD Vx, [I]
	Read registers V0 through Vx from memory starting at location I.
	The interpreter reads values from memory starting at location I into registers V0 through Vx.*/
void 
CHIP8::opF(WORD opcode)
{
	int x = (opcode & 0x0F00) >> 8;
	int n;
	int hundreds;
	int tens;
	int units;

	switch (opcode & 0x00FF)
	{
	case 0x0007:
		V[x] = delay_timer;
		break;
	case 0x000A:
		n = GetKeyPressed();
		if (n == -1)pc -= 2;
		else V[x] = n;
		break;
	case 0x0015:
		delay_timer = V[x];
		break;
	case 0x0018:
		sound_timer = V[x];
		break;
	case 0x001E:
		I += V[x];
		break;
	case 0x0029:
		n = V[x] * 5;
		I = memory[n];
		break;
	case 0x0033:
		n = V[x];
		hundreds = n / 100;
		tens = (n / 100) % 10;
		units = n % 10;

		memory[I] = hundreds;
		memory[I + 1] = tens;
		memory[I + 2] = tens;
		break;
	case 0x0055:
		for (int i = 0; i <= x; i++)
		{
			memory[I + i] = V[i];
		}
		I = I + x + 1;
		break;
	case 0x0065:
		for (int i = 0; i <= x; i++)
		{
			V[i] = memory[I + i];
		}
		I = I + x + 1;

		break;
	}
}




