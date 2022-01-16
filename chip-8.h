#pragma once
#include "renderer.h"
#include <stdint.h>

struct Chip8{
	Renderer *renderer;
	
	uint8_t  mem[0x1000];
	uint8_t  V[16]; // V[15] or VF is sometimes used as a flag.
	uint16_t I = 0x0000; // I register can only use 12 bits.
	uint16_t PC = 0x0000;
	uint16_t *SP;
	uint16_t stack[16];
	
	// This two timers are automatically decremented when they aren't zero.
	uint8_t DT = 0x00; // Delay timer;
	uint8_t ST = 0x00; // Sound timer;
	
	// const int width = 64;
	// const int height = 32;
	
};

void init_chip8(Chip8 *chip8, Renderer *renderer);
void emulator_loop(Chip8 *chip8);