#include "chip-8.h"
#include <stdio.h>

void init_chip8(Chip8 *chip8, Renderer *renderer){
	chip8->PC = 0x200;
	chip8->stack.SP = &chip8->stack.mem[0];
	chip8->renderer = renderer;
	
	chip8->V[5] = 255;
	chip8->V[1] = 3;
	
	init_memory_arena(&arena, ARENA_SIZE);
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



// static jump_to_address(uint16_t address, )

static void push_to_stack(Stack *stack, uint16_t address){
	// printf("Pushing address: %x", address);
	*(stack->SP) = address;
	// printf("Pushed address:  %x", *(stack->SP));
	stack->SP++;
}

static uint16_t pop_from_stack(Stack *stack){
	uint16_t popped = *(stack->SP);
	--stack->SP;
	return popped;
}

static void print_stack(Stack *stack){
	for(int i = 0; i < stack->SIZE; i++){
		printf("%x ", stack->mem[i]);
	}
}

static void print_registers(Chip8 *chip8){
	for(int i = 0; i < Chip8::NUM_REGISTERS; i++){
		printf("%d ", chip8->V[i]);
	}
	printf("\n");
}

uint16_t temp_PC = 0;
bool once = true;

uint8_t program[] = {
	0x50,0x10,
	0x00,0xE0,
	0x00,0xE0,
	0x81,0x54,
	0xFF,0xFF
};


static void emulator_interpret(Chip8 *chip8){
	uint8_t  first       = program[temp_PC]; // TODO: Change to the actual PC.
	uint8_t  second      = program[temp_PC + 1];
	uint16_t instruction = (second) | (first << 8);
	bool jumped = false;
	
	// printf("%x\n", instruction);
	
	if(once){
		printf("\n");
		printf("Instruction %x\n", instruction);
		// print_stack(&chip8->stack);
		
	}
	
	switch(instruction & 0xF000){
		case 0x0000:{
			switch(second){
				case 0xE0:{
					//Clear the screen.
					printf("Screen cleared\n");
					break;
				}
				
				case 0xEE:{
					//Return from subroutine.
					temp_PC = pop_from_stack(&chip8->stack);
					// temp_PC += 2; // To go to the next instruction.
					printf("Returned from subroutine to: %x\n", temp_PC + 2);
					break;
				}
			}
			
			break;
		}
		case 0x1000:{
			// Jump to address. 
			uint8_t high = first & 0x0F;
			temp_PC = (high << 8) | second;
			printf("Jumping to address: %x\n", temp_PC);
			jumped = true;
			
			break;
		}
		
		case 0x2000:{
			// Executes subroutine. Pushes the current address to the stack.
			push_to_stack(&chip8->stack, temp_PC);
			
			uint8_t high = first & 0x0F;
			temp_PC = (high << 8) | second;
			printf("Executing subroutine at: %x\n", temp_PC);
			jumped = true;
			
			break;
		}
		
		case 0x3000:{
			// Skip the next instrucion if the value of VX equals the lower 2 bytes.
			uint8_t x = first & 0x0F;
			uint8_t VX = chip8->V[x];
			if(VX == second){
				temp_PC += 2;
				printf("Skipping next instruction\n");
			}
			
			break;
		}
		
		case 0x4000:{
			// Skip the next instrucion if the value of VX is different than the lower 2 bytes.
			uint8_t x = first & 0x0F; 
			uint8_t VX = chip8->V[x];
			if(VX != second){
				temp_PC += 2;
				printf("Skipping next instruction\n");
			}
			
			break;
		}
		
		case 0x5000:{
			// Skip the next instrucion if the value of VX is different than VY.
			uint8_t x = first  & 0x0F; 
			uint8_t y = (second & 0xF0) >> 4;
			
			uint8_t VX = chip8->V[x];
			uint8_t VY = chip8->V[y];
			if(VX == VY){
				temp_PC += 2;
				printf("Skipping next instruction\n");
			}
			
			break;
		}
		
		case 0x6000:{
			// Store the lower 2 bytes in VX.
			uint8_t x = first  & 0x0F; 
			chip8->V[x] = second;
			printf("Storing value %d in V%d\n", second, x);
			
			break;
		}
		
		case 0x7000:{
			// Add the lower 2 bytes to VX.
			uint8_t x = first  & 0x0F; 
			chip8->V[x] += second;
			printf("Adding the value %d to V%d\n", second, x);
			
			break;
		}
		
		case 0x8000:{
			switch(second & 0x0F){
				case 0x0:{
					// Assign VY to VX.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					
					chip8->V[x] = chip8->V[y];
					printf("Assigning V%d to V%d\n", y, x);
					break;
				}
				
				case 0x1:{
					// Assign VX to VX OR VY.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					
					chip8->V[x] = VX | VY;
					printf("Assigning V%d to V%d OR V%d\n", y, x, y);
					break;
				}
				
				case 0x2:{
					// Assign VX to VX AND VY.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					
					chip8->V[x] = VX & VY;
					printf("Assigning V%d to V%d AND V%d\n", y, x, y);
					break;
				}
				
				case 0x3:{
					// Assign VX to VX XOR VY.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					
					chip8->V[x] = VX ^ VY;
					printf("Assigning V%d to V%d XOR V%d\n", y, x, y);
					break;
				}
				
				case 0x4:{
					// Add VY to VX and set VF to 01 if a carry occurs and 0 if not.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					uint16_t sum_with_carry = VX + VY;
					
					
					if(sum_with_carry >> 8){
						chip8->V[0xF] = 0x01;
					}else{
						chip8->V[0xF] = 0x00;
					}
					
					chip8->V[x] = VX + VY;
					printf("Adding V%d to V%d and setting VF if carry occurs\n", y, x);
					break;
				}
			}
			
			break;
		}
		
	}
	
	
	
	if(instruction == 0xFFFF){
		once = false; // This is to avoid looping. Just for testing purposes.
	} 
	if(once){
		if(!jumped){
			print_registers(chip8);
			temp_PC += 2; //Go to the next instruction. CHANGE THIS TO ACTUAL PC.
			// printf("Increase PC\n");
		}
		
	}
	
	
	
	// Decreasing the delay and sound timers.
	if(chip8->DT != 0) chip8->DT--;
	if(chip8->ST != 0) chip8->ST--;
}


void emulator_loop(Chip8 *chip8){
	emulator_interpret(chip8);
	renderer_draw(chip8->renderer);
    swap_buffers(chip8->renderer->window);
	poll_events();
}