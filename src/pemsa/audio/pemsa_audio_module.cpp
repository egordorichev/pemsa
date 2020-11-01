#include "pemsa/audio/pemsa_audio_module.hpp"
#include <cmath>

PemsaAudioModule::PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
}

PemsaAudioModule::~PemsaAudioModule() {
	delete this->backend;
}

double PemsaAudioModule::sample() {
	this->time += PEMSA_SAMPLE_OFFSET;
	return sin(this->time * (cos(this->time * 10) * 0.5 + 0.5));
}