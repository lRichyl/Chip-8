#include "timer.h"
#include "renderer.h"
#include "text.h"
#include "input.h"
#include "audio.h"
#include "chip-8.h"

#include "AL/alc.h"

#include <cmath>
#include <string>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_truetype.h"


void WindowResizeCallback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void WindowMaximizeCallback(GLFWwindow* window, int maximized){
	int width,height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

int main(int argc, char **argv){
	// First we get the name of the ROM via command line arguments.
	if(argc > 2){
		printf("Too many arguments, please provide only the rom's file name\n");
		return -1;
	}
	const char *rom_filename = argv[1];
	
	srand(time(NULL));
	Window *window;
	//The size here is the size of the window and the initial drawing resolution.
	//The internal resolution can be set manually and it's the one used for the game calculations.
	window = create_window(1280, 720, "Chip-8 Interpreter");
	window->internalWidth  = 64;
	window->internalHeight = 32;
	// You should also probably change the drawing resolution with the function which is the internal resolution
	// that is used to render to the framebuffer. This can benefit performance in lower values. The default value is 800x600.
	// change_drawing_resolution(Renderer *renderer, int width, int height).

	printf("%s \n",glGetString(GL_VERSION));
	Renderer *renderer = create_renderer(window);

	if(!renderer){
	  printf("Error creating the renderer\n");
	  exit(0);
	}

	float fixed_fps = 60.0f;
	// float dt_in_ms = 1.0f / fixed_fps * 1000.0f;
	float dt = 0;
	const int NUM_SAMPLES = 200;
	int sample_count = 0;
	float samples[NUM_SAMPLES] = {};

	//printf("%f", dt);

	long long perf_count_frequency = get_performance_counter_frequency();
	LARGE_INTEGER start_time = get_time_counter();

	
	// bool showFPS = true;
	// Texture font = make_texture("assets/fonts/font.png");
	// Text FPSText = Text(font, 12, V2{0,float(window->internalHeight)});
	glfwSetWindowSizeCallback(window->GLFWInstance,WindowResizeCallback);
	glfwSetWindowMaximizeCallback(window->GLFWInstance, WindowMaximizeCallback);
	glfwSwapInterval(1); //This should be set to 2 if the refresh rate of a monitor is 120Hz. If it's higher we need to come up with a factor.
	
	SetKeyboardCallback(window);
	SetCursorCallback(window);
	SetMouseButtonCallback(window);
	
	float average_fps;
	
	char *s = (char *)ALCCall(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
	ALCdevice *audio_device = NULL; 
	audio_device = ALCCall(alcOpenDevice(s));
	assert(audio_device);
	
	ALCcontext *audio_context = NULL;
	audio_context = ALCCall(alcCreateContext(audio_device, NULL ));
	assert(audio_context);
	ALCCall(alcMakeContextCurrent(audio_context));
	// char *s;
	
	// printf("default output device: %s\n", s);
	Chip8 chip8 {};
	init_chip8(&chip8, renderer);
	load_rom(rom_filename, &chip8);
	
	for(uint16_t i = 0x200; i < 0x1000; i+=2){
		printf("%x%x\n", chip8.mem[i], chip8.mem[i + 1]);
	}
	
	// float realfps ;
     while(!glfwWindowShouldClose(window->GLFWInstance)){


		//This is done because when we select the window it stops the execution of
		//the program so the next time we get the current time and calculate dt
		//it will be so big that physics break.
		// So if we grab the window for more than 100ms we skip the game loop.
		if(dt > 0 && dt < 0.1){
			Timer::dt = dt;
			emulator_loop(&chip8);
			update_audio_streams();
		}

		LARGE_INTEGER end_time = get_time_counter();
		float ms_per_frame = get_time_in_ms(start_time, end_time, perf_count_frequency);
		start_time = end_time;

		dt = ms_per_frame / 1000.0f;

		// if(game.showFPS && sample_count == NUM_SAMPLES - 1){
		   // float sum = 0;
		   // for(int i = 0; i < sample_count; i++){
				// sum += samples[i];
		   // }
		   // float average_ms = sum / (NUM_SAMPLES);
		   // average_fps = 1.f / (average_ms / 1000.f);
		   // sample_count = 0;


		// }
		// if(game.showFPS){
		   // samples[sample_count] = ms_per_frame;
		   // sample_count++;
		// }
		
     }
	 
	 alcDestroyContext(audio_context);
	 alcCloseDevice(audio_device);
     glfwTerminate();
     destroy_window(window);
     destroy_renderer(renderer);
     return 0;
}
