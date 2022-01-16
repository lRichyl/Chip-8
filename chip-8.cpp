#include "chip-8.h"
#include <stdio.h>

void init_chip8(Chip8 *chip8, Renderer *renderer){
	chip8->PC = 0x200;
	chip8->SP = &chip8->stack[0];
	chip8->renderer = renderer;
	
	// Load default FONT.
	//	0
	chip8->mem[0x0000] = 0xF0;
	chip8->mem[0x0001] = 0x90;
	chip8->mem[0x0002] = 0x90;
	chip8->mem[0x0003] = 0x90;
	chip8->mem[0x0004] = 0xF0;
	
	//	1
	chip8->mem[0x0005] = 0x20;
	chip8->mem[0x0006] = 0x60;
	chip8->mem[0x0007] = 0x20;
	chip8->mem[0x0008] = 0x20;
	chip8->mem[0x0009] = 0x70;
	
	//	2
	chip8->mem[0x000A] = 0xF0;
	chip8->mem[0x000B] = 0x10;
	chip8->mem[0x000C] = 0xF0;
	chip8->mem[0x000D] = 0x80;
	chip8->mem[0x000E] = 0xF0;
	
	//	3
	chip8->mem[0x000F] = 0xF0;
	chip8->mem[0x0010] = 0x10;
	chip8->mem[0x0011] = 0xF0;
	chip8->mem[0x0012] = 0x10;
	chip8->mem[0x0013] = 0xF0;
	
	//	4
	chip8->mem[0x0014] = 0x90;
	chip8->mem[0x0015] = 0x90;
	chip8->mem[0x0016] = 0xF0;
	chip8->mem[0x0017] = 0x10;
	chip8->mem[0x0018] = 0x10;
	
	//	5
	chip8->mem[0x0019] = 0xF0;
	chip8->mem[0x001A] = 0x80;
	chip8->mem[0x001B] = 0xF0;
	chip8->mem[0x001C] = 0x10;
	chip8->mem[0x001D] = 0xF0;
	
	//	6
	chip8->mem[0x001E] = 0xF0;
	chip8->mem[0x001F] = 0x80;
	chip8->mem[0x0020] = 0xF0;
	chip8->mem[0x0021] = 0x90;
	chip8->mem[0x0022] = 0xF0;
	
	//	7
	chip8->mem[0x0023] = 0xF0;
	chip8->mem[0x0024] = 0x10;
	chip8->mem[0x0025] = 0x20;
	chip8->mem[0x0026] = 0x40;
	chip8->mem[0x0027] = 0x40;
	
	//	8
	chip8->mem[0x0028] = 0xF0;
	chip8->mem[0x0029] = 0x90;
	chip8->mem[0x002A] = 0xF0;
	chip8->mem[0x002B] = 0x90;
	chip8->mem[0x002C] = 0xF0;
	
	//	9
	chip8->mem[0x002D] = 0xF0;
	chip8->mem[0x002E] = 0x90;
	chip8->mem[0x002F] = 0xF0;
	chip8->mem[0x0030] = 0x10;
	chip8->mem[0x0031] = 0xF0;
	
	//	A
	chip8->mem[0x0032] = 0xF0;
	chip8->mem[0x0033] = 0x90;
	chip8->mem[0x0034] = 0xF0;
	chip8->mem[0x0035] = 0x90;
	chip8->mem[0x0036] = 0x90;
	
	//	B
	chip8->mem[0x0037] = 0xE0;
	chip8->mem[0x0038] = 0x90;
	chip8->mem[0x0039] = 0xE0;
	chip8->mem[0x003A] = 0x90;
	chip8->mem[0x003B] = 0xE0;
	
	//	C
	chip8->mem[0x003C] = 0xF0;
	chip8->mem[0x003D] = 0x80;
	chip8->mem[0x003E] = 0x80;
	chip8->mem[0x003F] = 0x80;
	chip8->mem[0x0040] = 0xF0;
	
	//	D
	chip8->mem[0x0041] = 0xE0;
	chip8->mem[0x0042] = 0x90;
	chip8->mem[0x0043] = 0x90;
	chip8->mem[0x0044] = 0x90;
	chip8->mem[0x0045] = 0xE0;
	
	//	E
	chip8->mem[0x0046] = 0xF0;
	chip8->mem[0x0047] = 0x80;
	chip8->mem[0x0048] = 0xF0;
	chip8->mem[0x0049] = 0x80;
	chip8->mem[0x004A] = 0xF0;
	
	//	F
	chip8->mem[0x004B] = 0xF0;
	chip8->mem[0x004C] = 0x80;
	chip8->mem[0x004D] = 0xF0;
	chip8->mem[0x004E] = 0x80;
	chip8->mem[0x004F] = 0x80;
}

uint8_t program[] = {
	0x10,0x10,
	0x10,0x20,
	0xFF,0xFF
};

int temp_PC = 0;
bool once = true;
static void emulator_interpret(Chip8 *chip8){
	uint8_t  first       = program[temp_PC]; // TODO: Change to the actual PC.
	uint8_t  second      = program[temp_PC + 1];
	uint16_t instruction = (second) | (first << 8);
	
	switch(instruction & 0x1000){
		case 0x1000:{
			switch(instruction){
				case 0x1010:{
					printf("TEST 1\n");
					break;
				}
				case 0x1020:{
					printf("TEST 2\n");
					break;
				}
			}
			
			break;
		}
	}
	
	if(instruction == 0xFFFF) once = false;
	
	if(once){
		temp_PC += 2; //Go to the next instruction. CHANGE THIS TO ACTUAL PC.
		printf("Instruction %x", first);
		printf("%x\n", second);
	}
}


void emulator_loop(Chip8 *chip8){
	emulator_interpret(chip8);
	renderer_draw(chip8->renderer);
    swap_buffers(chip8->renderer->window);
	poll_events();
}