#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include "sdl/sdl_audio_backend.hpp"
#include <iostream>

static void fillBuffer(void* emulator, Uint8* byteStream, int byteStreamLength) {
	uint16_t* buffer = (uint16_t*) byteStream;

	PemsaAudioModule* audioModule = ((PemsaEmulator*) emulator)->getAudioModule();

	for (int i = 0; i < byteStreamLength / 2; i++) {
		buffer[i] = audioModule->sample() * INT16_MAX;
	}
}

SdlAudioBackend::~SdlAudioBackend() {
	if (this->stream != 0) {
		SDL_CloseAudioDevice(this->stream);
	}
}

void SdlAudioBackend::setEmulator(PemsaEmulator *emulator) {
	PemsaBackend::setEmulator(emulator);
	this->setup();
}

void SdlAudioBackend::setup() {
	SDL_AudioSpec want;
	SDL_AudioSpec audioSpec;

	SDL_zero(want);
	SDL_zero(audioSpec);

	want.freq = PEMSA_SAMPLE_RATE;
	want.format = AUDIO_S16LSB;
	want.channels = 1;
	want.samples = PEMSA_MIN_BYTES_IN_AUDIO;
	want.userdata = this->emulator;
	want.callback = fillBuffer;

	this->stream = SDL_OpenAudioDevice(NULL, 0, &want, &audioSpec, 0);

	if (this->stream == 0) {
		std::cerr << "Failed to open audio: " << SDL_GetError() << "\n";
	} else {
		SDL_PauseAudioDevice(this->stream, false);
	}
}