#include "sdl/sdl_graphics_backend.hpp"
#include "sdl/sdl_audio_backend.hpp"
#include "sdl/sdl_input_backend.hpp"

#include "pemsa/pemsa.hpp"
#include "SDL2/SDL.h"

int main() {
	PemsaEmulator emulator(new SdlGraphicsBackend(), new SdlAudioBackend(), new SdlInputBackend());

	return 0;
}