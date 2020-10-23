#include "sdl/sdl_graphics_backend.hpp"
#include "sdl/sdl_audio_backend.hpp"
#include "sdl/sdl_input_backend.hpp"

#include "pemsa/pemsa.hpp"
#include "SDL2/SDL.h"

#include <iostream>

int main(int argc, const char** argv) {
	const char* cart = "carts/test.p8";

	if (argc > 1) {
		cart = argv[1];
	}

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("pemsa", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512,512, SDL_WINDOW_OPENGL);

	if (window == NULL) {
		std::cerr << "Failed to open a window\n";
		SDL_Quit();

		return 1;
	}

	SdlGraphicsBackend* graphics = new SdlGraphicsBackend(window);
	PemsaEmulator emulator(graphics, new SdlAudioBackend(), new SdlInputBackend());

	if (!emulator.getCartridgeModule()->load(cart)) {
		std::cerr << "Failed to load the cart " << cart << "\n";

		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}

	SDL_Surface* screen_surface = SDL_GetWindowSurface(window);

	SDL_Event event;
	bool running = true;

	Uint32 start_time = SDL_GetTicks();

	double fps = 60.0;
	double delta = 1 / fps;
	Uint32 ticks_per_frame = 1000 / fps;

	SDL_Rect target_size = { 0, 0, 512, 512 };

	while (running) {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		emulator.update(delta);

		SDL_BlitScaled(graphics->getSurface(), NULL, screen_surface, &target_size);
		SDL_UpdateWindowSurface(window);

		Uint32 end_time = SDL_GetTicks();
		Uint32 difference = end_time - start_time;

		if (difference < ticks_per_frame) {
			SDL_Delay(ticks_per_frame - difference);

			end_time = SDL_GetTicks();
			difference = end_time - start_time;
		}

		delta = difference / 1000.0;
		fps = 1000.0 / difference;

		start_time = end_time;
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}