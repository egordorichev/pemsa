#include "pemsa/audio/pemsa_audio_module.hpp"
#include <cmath>

PemsaAudioModule::PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
}

PemsaAudioModule::~PemsaAudioModule() {
	delete this->backend;
}

int16_t* PemsaAudioModule::getBuffer() {
	for (int i = 0; i < PEMSA_SAMPLE_SIZE; i++) {
		this->buffer[i] = sin(this->time * 220);
		this->time += (double) i / PEMSA_SAMPLE_SIZE;
	}

	return this->buffer;
}

void PemsaAudioModule::update(double dt) {
	this->backend->update();
}
