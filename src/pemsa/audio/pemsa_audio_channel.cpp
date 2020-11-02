#include "pemsa/audio/pemsa_audio_channel.hpp"
#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/audio/pemsa_wave_functions.hpp"
#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <iostream>

PemsaAudioChannel::PemsaAudioChannel(PemsaEmulator* emulator, int id) {
	this->emulator = emulator;
	this->id = id;
	this->active = false;
}

void PemsaAudioChannel::play(int sfx) {
	this->sfx = sfx;
	this->offset = 0;
	this->lastStep = -1;
	this->loop = false;
	this->active = true;
	this->speed = this->emulator->getMemoryModule()->ram[sfx * 68 + PEMSA_RAM_SFX + 65];
}

double PemsaAudioChannel::sample() {
	if (!this->active) {
		return 0;
	}

	uint8_t* ram = this->emulator->getMemoryModule()->ram;

	if ((int) this->offset > this->lastStep) {
		this->lastNote = this->note;

		if (this->offset > 32) {
			this->offset = 0;
			this->lastStep = -1;
		} else {
			this->lastStep = (int) this->offset;
		}

		int i = this->sfx * 68 + PEMSA_RAM_SFX + (int) this->offset * 2;

		uint8_t lo = ram[i];
		uint8_t hi = ram[i + 1];

		this->note = (lo & 0b00111111);
		this->frequency = NOTE_TO_FREQUENCY(this->note);

		this->instrument = (uint8_t) (((lo & 0b11000000) >> 6) | ((hi & 0b1) << 2));
		this->volume = (uint8_t) ((hi & 0b00001110) >> 1);
		this->fx = (uint8_t) ((hi & 0b01110000) >> 4);
		this->isCustom = (uint8_t) ((hi & 0b10000000) >> 7) == 1;
	}

	this->offset += 7350.0 / (61 * this->speed * PEMSA_SAMPLE_RATE);
	this->waveOffset += this->frequency / PEMSA_SAMPLE_RATE;

	if (this->volume == 0) {
		return 0;
	}

	return pemsa_sample(this->instrument, this->waveOffset) * (this->volume / 7.0);
}

bool PemsaAudioChannel::isActive() {
	return this->active;
}
