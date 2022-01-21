#include "chip-8.h"
#include "input.h"
#include <stdio.h>
#include <cmath>
#include <time.h>

void init_chip8(Chip8 *chip8, Renderer *renderer){
	srand(time(NULL));
	chip8->framebuffer = make_texture(4, 64, 32);
	chip8->PC = 0x200;
	chip8->stack.SP = &chip8->stack.mem[0];
	chip8->renderer = renderer;
	
	// chip8->V[0] = 253;
	// chip8->V[1] = 15;
	// chip8->V[2] = 22;
	// chip8->V[3] = 120;
	// chip8->V[4] = 34;
	
	// chip8->DT = 0xF;
	
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
	
	// Key pad values mapped to GLFW key codes.
	
	chip8->key_pad[0x1] = GLFW_KEY_1;
	chip8->key_pad[0x2] = GLFW_KEY_2;
	chip8->key_pad[0x3] = GLFW_KEY_3;
	chip8->key_pad[0xC] = GLFW_KEY_4;
	
	chip8->key_pad[0x4] = GLFW_KEY_Q;
	chip8->key_pad[0x5] = GLFW_KEY_W;
	chip8->key_pad[0x6] = GLFW_KEY_E;
	chip8->key_pad[0xD] = GLFW_KEY_R;
	
	chip8->key_pad[0x7] = GLFW_KEY_A;
	chip8->key_pad[0x8] = GLFW_KEY_S;
	chip8->key_pad[0x9] = GLFW_KEY_D;
	chip8->key_pad[0xE] = GLFW_KEY_F;
	
	chip8->key_pad[0xA] = GLFW_KEY_Z;
	chip8->key_pad[0x0] = GLFW_KEY_X;
	chip8->key_pad[0xB] = GLFW_KEY_C;
	chip8->key_pad[0xF] = GLFW_KEY_V;
	
}



// static jump_to_address(uint16_t address, )

static void push_to_stack(Stack *stack, uint16_t address){
	// printf("Pushing address: %x", address);
	*(stack->SP) = address;
	// printf("Pushed address:  %x", *(stack->SP));
	stack->SP++;
}

static uint16_t pop_from_stack(Stack *stack){
	--stack->SP;
	uint16_t popped = *(stack->SP);
	return popped;
}

static void print_stack(Stack *stack){
	for(int i = 0; i < stack->SIZE; i++){
		printf("%x ", stack->mem[i]);
	}
}

static void print_registers(Chip8 *chip8){
	for(int i = 0; i < Chip8::NUM_REGISTERS; i++){
		printf("%x ", chip8->V[i]);
	}
	printf("\n");
}


uint8_t program[] = {
	0xF4,0x65,
	0xA0,0x06,
	0xD0,0x15,
	0xFF,0x80,
	0xFF,0x80,
	0xFF,0x80,
	0x80,0x00,
	0xFF,0xFF
};

static Rect r = {0, 32, 1, 1};
static Rect r1 = {0, 30, 2, 2};

static bool is_within_screen_bounds(V2 pos){
	if(pos.x >= 0 && pos.x < Chip8::WIDTH && pos.y >= 0 && pos.y < Chip8::HEIGHT){
		return true;
	}
	return false;
}

static void emulator_interpret(Chip8 *chip8){
	uint8_t  first       = chip8->mem[chip8->PC];
	uint8_t  second      = chip8->mem[chip8->PC + 1];
	uint16_t instruction = (second) | (first << 8);
	bool jumped = false;
	
	// if(once){
		// printf("\n");
		printf("Instruction %x\n", instruction);
		// print_stack(&chip8->stack);
		
	// }
	
	switch(instruction & 0xF000){
		case 0x0000:{
			switch(second){
				case 0xE0:{
					//Clear the screen.
					clear_texture(&chip8->framebuffer);
					printf("Screen cleared\n");
					break;
				}
				
				case 0xEE:{
					//Return from subroutine.
					chip8->PC = pop_from_stack(&chip8->stack);
					// jumped = true;
					// chip8->PC += 2;
					printf("Returned from subroutine to: %x\n", chip8->PC + 2);
					break;
				}
			}
			
			break;
		}
		// static uint16_t last_address = 0;
		case 0x1000:{
			// Jump to address. 
			uint8_t high = first & 0x0F;
			chip8->PC = (high << 8) | second;
			printf("Jumping to address: %x\n", chip8->PC);
			jumped = true;
			
			// if(last_address == chip8->PC && chip8->PC != 0) jumped = false;
			
			// last_address = chip8->PC;
			break;
		}
		
		case 0x2000:{
			// Executes subroutine. Pushes the current address to the stack.
			push_to_stack(&chip8->stack, chip8->PC);
			
			uint8_t high = first & 0x0F;
			chip8->PC = (high << 8) | second;
			printf("Executing subroutine at: %x\n", chip8->PC);
			jumped = true;
			
			break;
		}
		
		case 0x3000:{
			// Skip the next instrucion if the value of VX equals the lower 2 bytes.
			uint8_t x = first & 0x0F;
			uint8_t VX = chip8->V[x];
			if(VX == second){
				chip8->PC += 2;
				printf("Skipping next instruction\n");
			}
			
			break;
		}
		
		case 0x4000:{
			// Skip the next instrucion if the value of VX is different than the lower 2 bytes.
			uint8_t x = first & 0x0F; 
			uint8_t VX = chip8->V[x];
			if(VX != second){
				chip8->PC += 2;
				printf("Skipping next instruction\n");
			}
			
			break;
		}
		
		case 0x5000:{
			// Skip the next instrucion if the value of VX is equal to the value in VY.
			uint8_t x = first  & 0x0F; 
			uint8_t y = (second & 0xF0) >> 4;
			
			uint8_t VX = chip8->V[x];
			uint8_t VY = chip8->V[y];
			if(VX == VY){
				chip8->PC += 2;
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
					printf("Assigning V%d to V%d AND V%d\n", x, x, y);
					break;
				}
				
				case 0x3:{
					// Assign VX to VX XOR VY.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					
					chip8->V[x] = VX ^ VY;
					printf("Assigning V%d to V%d XOR V%d\n", x, x, y);
					break;
				}
				
				case 0x4:{
					// Add VY to VX and set VF to 01 if a carry occurs and 0 if not.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					uint16_t sum_with_carry = (uint16_t)VX + (uint16_t)VY;
					
					
					if(sum_with_carry > 0xFF){
						chip8->V[0xF] = 0x01;
					}else{
						chip8->V[0xF] = 0x00;
					}
					
					chip8->V[x] = VX + VY;
					printf("Adding V%d to V%d and setting VF if carry occurs\n", y, x);
					break;
				}
				
				case 0x5:{
					// Substract VY from VX and set VF to 0 if a borrow occurs and 1 if not.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					
					chip8->V[0xF] = 0x00;
					if(VX > VY){
						chip8->V[0xF] = 0x01;
					}
					// else if(VY > VX){
						// chip8->V[0xF] = 0x00;
					// }
					
					// if(VX == VY){
						// chip8->V[0xF] = 0x01;
					// }
					
					chip8->V[x] = VX - VY;
					printf("Substacting V%d from V%d and setting VF if a borrow doesn't occur\n", y, x);
					break;
				}
				
				case 0x6:{
					// Set VX to VY >> 1 and set VF to the least significant bit of VX before the shift.
					uint8_t x = first  & 0x0F; 
					// uint8_t y = (second & 0xF0) >> 4;
					// uint8_t VY = chip8->V[y];
					
					// chip8->V[x] = chip8->V[y];
					chip8->V[0xF] = chip8->V[x] & 0x01;
					chip8->V[x] = chip8->V[x] >> 1;
					printf("Setting V%d to V%d/2 and VF to the least significant bit before the shift\n", x, x);
					break;
				}
				
				case 0x7:{
					// Substract VX from VY and set VF to 0 if a borrow occurs and 1 if not.
					uint8_t x = first  & 0x0F; 
					uint8_t y = (second & 0xF0) >> 4;
					uint8_t VX = chip8->V[x];
					uint8_t VY = chip8->V[y];
					
					chip8->V[0xF] = 0x00;
					chip8->V[x] = VY - VX;
					if(VY > VX){
						chip8->V[0xF] = 0x01;
					}
					// else if(VX > VY){
						// chip8->V[0xF] = 0x00;
					// }
					// if(VX == VY){
						// chip8->V[0xF] = 0x01;
					// }
					// chip8->V[0xF] = VX & 0x01;
					printf("Substacting V%d from V%d and setting VF if a borrow doesn't occur\n", x, y);
					break;
				}
				
				case 0xE:{
					// Set VX to VY and then << 1 and set VF to the most significant bit of VX before the shift.
					uint8_t x = first  & 0x0F; 
					// uint8_t y = (second & 0xF0) >> 4;
					// uint8_t VY = chip8->V[y];
					
					// chip8->V[x] = chip8->V[y];
					chip8->V[0xF] = (chip8->V[x] & 0x80) >> 7;
					chip8->V[x] = chip8->V[x] << 1;
					printf("Setting V%d to %x and VF to the most significant bit before the shift\n", x, chip8->V[x]);
					break;
				}
			}
			
			break;
		}
		
		case 0x9000:{
			// Skip the next instruction if the value in VX is different than the value in VY.
			uint8_t x = first  & 0x0F;
			uint8_t y = (second & 0xF0) >> 4;
			
			if(chip8->V[x] != chip8->V[y]){
				chip8->PC += 2;
			}
			
			printf("Skipping the next instruction if V%d is different than V%d\n", x, y);
			break;
		}
		
		case 0xA000:{
			// Sets the I register to the lower 12 bits of the instruction.
			uint16_t address = second | ((first & 0x0F) << 8);
			chip8->I = address;
			printf("Setting I to the lower 12 bits %x\n", address);
			break;
		}
		
		case 0xB000:{
			// Jump to address equal to the lower 12 bits plus the value in V0.
			uint16_t address = second | ((first & 0x0F) << 8);
			// uint8_t x = first  & 0x0F; 
			address += (uint16_t)chip8->V[0];
			printf("Jumping to address %x + V%d: %x\n", address, address, chip8->V[0]);
			chip8->PC = address;
			jumped = true;
			break;
		}
		
		case 0xC000:{
			// Setting the value of register VX to a random number ANDed with the lower 2 bytes.
			uint8_t x = first  & 0x0F; 
			uint8_t random = rand();
			chip8->V[x] = random & second;
			printf("Setting VX to the result of random (%x) AND %x\n", random, second);
			break;
		}
		
		case 0xD000:{
			// Displays a sprite at position VX, VY, N bytes size(height), starting at the address stored in the register I.
			// It XORs the current pixel being displayed with the new one and if the result is 0 VF is set to 1. 
			//This means that if you draw the same sprite twice at the same location, the sprite will be erased.
 			uint8_t  x       = first  & 0x0F; 
			uint8_t  y       = (second & 0xF0) >> 4;
			uint8_t  bytes   = second & 0x0F;
			uint8_t  x_pos   = chip8->V[x];
			uint8_t  y_pos   = chip8->V[y];
			uint16_t start_address = chip8->I;
			printf("Start address: %x\n", start_address);

			printf("Drawing a sprite at position %d, %d and setting VF if any pixel is set to unset\n", x_pos, y_pos);
			
			// The chip8 starts its screen coordinates at the top left corner, but in our renderer we have the point 0,0 at the 
			// bottom down corner so we need to convert to our coordinate system.
			// So we substract the y position from the display height.
			
			bool set_VF = false;
			if(!is_within_screen_bounds(V2 {(float)x_pos, (float)Chip8::HEIGHT - y_pos - 1})){ // If the origin is out of bounds we wrap the coordinates.
				if(x_pos >= Chip8::WIDTH)  x_pos = (x_pos % Chip8::WIDTH);
				if(y_pos >= Chip8::HEIGHT) y_pos = (y_pos % Chip8::HEIGHT);
			}
			
			for(uint8_t j = 0; j < bytes; j++){
				uint8_t current_byte = chip8->mem[start_address + j];
				// printf("Current byte: %x\n", current_byte);
				for(uint8_t i = 0; i < 8; i++){
					V2 pixel_pos = {(float)x_pos + i, (float)((Chip8::HEIGHT) - (y_pos + j) - 1)};
					
					int displacement = 8 - i - 1;
					uint8_t pixel = (current_byte & (1 << displacement)) >> (displacement);
					if(pixel == 0) continue; // If the bits is 0 we ignore it.
					
					// If one of the pixels coordinates is out of bounds the pixel gets clipped. 
					if(is_within_screen_bounds(pixel_pos)){
						V4 current_pixel = get_pixel(&chip8->framebuffer, pixel_pos);
						uint8_t current_pixel_bit;
						if(current_pixel.x == 255){
							current_pixel_bit = 1;
						}else if(current_pixel.x == 0){
							current_pixel_bit = 0;
						}
						
						uint8_t result = pixel ^ current_pixel_bit;
						if(result == 1){
							// set_VF = false;
							set_pixel(&chip8->framebuffer, V4{255,255,255,255}, pixel_pos);
						}else if(result == 0){
							set_VF = true;
							set_pixel(&chip8->framebuffer, V4{0,0,0,0}, pixel_pos);
						}
					}
					
				}
			}
			if(set_VF){
				chip8->V[0xF] = 1;
			}else{
				chip8->V[0xF] = 0;
			}
			
			render_quad(chip8->renderer, NULL, &chip8->framebuffer, 0);
			renderer_draw(chip8->renderer);
			swap_buffers(chip8->renderer->window);
			
			break;
		}
		
		case 0xE000:{
			switch(second){
				case 0x9E:{
					// Skip the next instruction if the key corresponding to the value stored in VX is pressed.
					uint8_t x  = first & 0x0F; 
					uint8_t VX = chip8->V[x];
					int32_t key_code = chip8->key_pad[VX];
					if(IsKeyPressed(chip8->renderer->window, key_code)){
						chip8->PC += 2;
						
					}
					printf("Skip next instruction if the key %x is pressed\n", VX);
					break;
				}
				
				case 0xA1:{
					// Skip the next instruction if the key corresponding to the value stored in VX is not pressed.
					uint8_t x  = first & 0x0F; 
					uint8_t VX = chip8->V[x];
					int32_t key_code = chip8->key_pad[VX];
					if(!IsKeyPressed(chip8->renderer->window, key_code)){
						chip8->PC += 2;
						
					}
					printf("Skip next instruction if the key %x is not pressed\n", VX);
					break;
				}
			}
			break;
		}
		
		case 0xF000:{
			switch(second){
				case 0x07:{
					uint8_t x = first & 0x0F;
					chip8->V[x] = chip8->DT;
					printf("Storing the value of the Delay Timer in V%d\n", x);
					break;
				}
				
				case 0x0A:{
					uint8_t x = first & 0x0F;
					// chip8->V[x] = chip8->DT;
					while(true){
						for(int i = 0; i <= Chip8::NUM_KEYS; i++){
							int32_t key_code = chip8->key_pad[i];
							if(IsKeyPressed(chip8->renderer->window, key_code)){
								chip8->V[x] = i;
								goto key_pressed;
							}	
						}
						poll_events();
					}
					key_pressed:
					
					printf("Waited for a key press and stored the result in V%d\n", x);
					break;
				}
				
				case 0x15:{
					uint8_t x = first & 0x0F;
					chip8->DT = chip8->V[x];
					printf("Set the Delay Timer to the value in V%d\n", x);
					break;
				}
				
				case 0x18:{
					uint8_t x = first & 0x0F;
					chip8->ST = chip8->V[x];
					printf("Set the Sound Timer to the value in V%d\n", x);
					break;
				}
				
				case 0x1E:{
					uint8_t x = first & 0x0F;
					chip8->I += chip8->V[x];
					printf("Add the value of register V%d to the register I\n", x);
				}
				
				case 0x29:{
					uint8_t x = first & 0x0F;
					chip8->I = chip8->V[x] * 5;
					printf("Set the value of register I to the address of the corresponding font character of the digit stored in V%d Address: %x\n", x, chip8->I);
					break;
				}
				
				case 0x33:{
					uint8_t x = first & 0x0F;
					uint8_t VX = chip8->V[x];
					
					uint8_t hundreds =  VX / 100;
					uint8_t tens     = (VX / 10) % 10;
					uint8_t units    = ((VX % 100) % 10);
					
					uint16_t I = chip8->I;
					chip8->mem[I]     = hundreds;
					chip8->mem[I + 1] = tens;
					chip8->mem[I + 2] = units;
					printf("Store the BCD equivalent of the value stored in V%d in the address I, I + 1, and I + 2\n", x);
					printf("%x, %x, %x\n", chip8->mem[I], chip8->mem[I + 1], chip8->mem[I + 2]);
					break;
				}
				
				case 0x55:{
					uint8_t  x = first & 0x0F;
					uint16_t I = chip8->I;
					
					for(int i = 0; i <= x; i++){
						chip8->mem[I + i] = chip8->V[i];
					}
					printf("Storing the values V0 through V%d starting at address I\n", x);
					
					for(int i = 0; i <= x; i++){
						printf("%d, ", chip8->mem[I + i]);
					}
					chip8->I += x + 1;
					printf("\n");
					break;
				}
				
				case 0x65:{
					uint8_t  x = first & 0x0F;
					uint16_t I = chip8->I;
					
					for(int i = 0; i <= x; i++){
						chip8->V[i] = chip8->mem[I + i];
					}
					printf("Storing values starting at address %x in registers V0 through V%d\n", I, x);
					
					for(int i = 0; i <= x; i++){
						printf("%d, ", chip8->mem[I + i]);
					}
					chip8->I += x + 1;
					printf("\n");
					break;
				}
			}
			
			break;
		}
		default:
			printf("Unknown instruction\n");
			exit(-1);
	}
	
	
	
	// if(instruction == 0xFFFF){
		// once = false; // This is to avoid looping. Just for testing purposes.
		// chip8->PC = 0;
		// jumped = true;
	// } 
	// if(once){
	if(!jumped){
		print_registers(chip8);
		chip8->PC += 2; //Go to the next instruction. CHANGE THIS TO ACTUAL PC.
			// printf("Increase PC\n");
	}
		
	// }
	
	
	
	
}


void emulator_loop(Chip8 *chip8){
	// The main loop executes at 60Hz. So we execute 8 instuctions in one cycle which means that the instructions
	// get executed at a rate of 480Hz. This is done to get a comfortable speed for playing games.
	for(int i = 0; i < 8; i++){
		emulator_interpret(chip8);
		
		poll_events();
	}
	// Decreasing the delay and sound timers.
	if(chip8->DT != 0) chip8->DT--;
	if(chip8->ST != 0) chip8->ST--;
	
	
	
	
}

void load_rom(const char *filename, Chip8 *chip8){
	FILE *file = fopen(filename, "rb");
	if(!file){
		printf("File: %s could not be opened\n", filename);
		exit(-1);
	}
	fseek(file, 0L, SEEK_END);
	int size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	// rewind(file);
	// printf("%d\n", size);
	
	uint16_t counter = 0;
	for(int i = 0; i < size; i++){
		uint8_t byte_high = getc(file);
		uint8_t byte_low  = getc(file);
		
		if(byte_high == 0xFF && byte_low == 0xFF) break;
		// fscanf(file, "%x%x", &byte_high, &byte_low);
		chip8->mem[0x200 + counter]     = byte_high;
		chip8->mem[0x200 + counter + 1] = byte_low;
		// printf("%x%x\n", chip8->mem[0x200 + counter], chip8->mem[0x200 + counter + 1]);
		counter += 2;
		// printf("%x\n",chip8->mem[0x200]);
		// printf("%x%x\n", byte_high, byte_low);
	}
	fclose(file);
}