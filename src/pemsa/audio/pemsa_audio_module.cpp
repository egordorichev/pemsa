#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/audio/pemsa_wave_functions.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <cmath>

PemsaAudioModule::PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		this->channels[i] = new PemsaAudioChannel(emulator, i);
	}
}

PemsaAudioModule::~PemsaAudioModule() {
	delete this->backend;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		delete this->channels[i];
	}
}

double PemsaAudioModule::sample() {
	double result = 0;
	int count = 0;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		PemsaAudioChannel* channel = this->channels[i];

		// fixme: segfault sometimes
		if (channel->isActive()) {
			result += channel->sample();
			count++;
		}
	}

	return result / count;
}

void PemsaAudioModule::play(int sfx) {
	this->channels[0]->play(sfx);
}
