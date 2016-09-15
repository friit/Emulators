#pragma once
#include "Macros.h"
#include <stdio.h>
#include <vector>

 class CHIP8 {

	public:
		//Set up CPU Registers and memory

		//An opcode is 2 bytes
		WORD opcode;

		//Chip8 has 4k of memory
		BYTE memory[0xFFF];
		BYTE V[16];

		//Index and program counter
		WORD I;
		WORD pc;

		//Chip8 has a screen of 2048 pixels
		BYTE gfx[320][640][3];

		//Chip8 has two timer registers 
		BYTE delay_timer;
		BYTE sound_timer;


		//Chip8 has a 16 block stack
		std::vector<WORD> Stack;
		WORD sp;

		//Chip8 hasa hex based keyboard
		BYTE key[16];

		//Functions

		bool initialize();
		bool loadgame(std::string game);
		bool draw;
		void emulateCycle();
		void run();
		unsigned char chip8_fontset[80];

		WORD FetchNextOpcode();
		void DecodeOpcode(WORD opcode);

		void op0(WORD opcode);
		void op1(WORD opcode);
		void op2(WORD opcode);
		void op3(WORD opcode);
		void op4(WORD opcode);
		void op5(WORD opcode);
		void op6(WORD opcode);
		void op7(WORD opcode);
		void op8(WORD opcode);
		void op9(WORD opcode);
		void opA(WORD opcode);
		void opB(WORD opcode);
		void opC(WORD opcode);
		void opD(WORD opcode);
		void opE(WORD opcode);
		void opF(WORD opcode);

		void KeyPressed(int i);
		void KeyReleased(int i);
		int GetKeyPressed();


	};