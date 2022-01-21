#pragma once
#include "renderer.h"
#include "memory_arena.h"
#include <stdint.h>

static const int ARENA_SIZE = 1024;
static MemoryArena arena;


struct Stack{
	static const int SIZE = 17;
	uint16_t *SP;
	uint16_t mem[SIZE] {};
};

struct Chip8{
	Renderer *renderer;
	Texture framebuffer;
	
	static const int NUM_REGISTERS = 16;
	static const int WIDTH = 64;
	static const int HEIGHT = 32;
	static const int NUM_KEYS = 16;
	
	uint8_t  mem[0x1000] {};
	uint8_t  V[NUM_REGISTERS]{}; // V[15] or VF is sometimes used as a flag.
	uint16_t I = 0x0000; // I register can only use 12 bits.
	uint16_t PC = 0x0000;
	Stack stack;
	
	// This two timers are automatically decremented at a rate of 60Hz when they aren't zero.
	uint8_t DT = 0x00; // Delay timer;
	uint8_t ST = 0x00; // Sound timer;
	
	int32_t key_pad[NUM_KEYS];
	
};

void load_rom(const char *filename, Chip8 *chip8);
void init_chip8(Chip8 *chip8, Renderer *renderer);
void emulator_loop(Chip8 *chip8);