#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include "sdl/sdl_audio_backend.hpp"
#include <iostream>

SdlAudioBackend::SdlAudioBackend() {
	this->stream = SDL_NewAudioStream(AUDIO_S16, 1, PEMSA_SAMPLE_SIZE / 2, AUDIO_F32, 2, PEMSA_SAMPLE_RATE);
}

SdlAudioBackend::~SdlAudioBackend() {
	SDL_FreeAudioStream(this->stream);
}

void SdlAudioBackend::update() {
	int available = SDL_AudioStreamAvailable(this->stream);

	while (available < PEMSA_MIN_BYTES_IN_AUDIO) {
		available += PEMSA_SAMPLE_SIZE;

		if (SDL_AudioStreamPut(stream, this->emulator->getAudioModule()->getBuffer(), PEMSA_SAMPLE_SIZE * sizeof(int16_t)) == -1) {
			std::cerr << "Failed to submit audio buffer: " << SDL_GetError() << "\n";
		}
	}
}