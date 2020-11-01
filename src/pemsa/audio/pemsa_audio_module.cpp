#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/audio/pemsa_wave_functions.hpp"

#include <cmath>

PemsaAudioModule::PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
}

PemsaAudioModule::~PemsaAudioModule() {
	delete this->backend;
}

double PemsaAudioModule::sample() {
	double frequency = 220 + fmod(this->time, 1000);

	this->time += frequency / PEMSA_SAMPLE_RATE;
	return pemsa_noise(this->time);
}