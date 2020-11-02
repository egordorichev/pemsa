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
	this->note = 0;
	this->lastNote = 0;
	this->sfx = sfx;
	this->offset = 0;
	this->lastStep = -1;
	this->loop = false;
	this->active = true;
	this->speed = this->emulator->getMemoryModule()->ram[sfx * 68 + PEMSA_RAM_SFX + 65];
}

static double lerp(double a, double b, double t) {
	return (b - a) * t + a;
}

static double osc(double x) {
	return (fabs(fmod(x, 1) * 2 - 1) * 2 - 1) * 0.7;
}

static double sawOsc(double x) {
	return fmod(x, 1);
}

double PemsaAudioChannel::sample() {
	if (!this->active) {
		return 0;
	}

	if ((int) this->offset > this->lastStep) {
		uint8_t* ram = this->emulator->getMemoryModule()->ram;
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

	double vol = this->volume;

	switch (this->fx) {
		case 1: {
			this->frequency = lerp(NOTE_TO_FREQUENCY(this->lastNote), NOTE_TO_FREQUENCY(this->note), fmod(this->offset, 1.0));
			break;
		}

		case 2: {
			this->frequency = lerp(NOTE_TO_FREQUENCY(this->note), NOTE_TO_FREQUENCY(this->note + 0.5), osc(this->time * 8));
			break;
		}

		case 3: {
			this->frequency = lerp(NOTE_TO_FREQUENCY(this->lastNote), 0, fmod(this->offset, 1.0));
			break;
		}

		case 4: {
			vol = lerp(0, (double) this->volume, fmod(this->offset, 1.0));
			break;
		}

		case 5: {
			vol = lerp((double) this->volume, 0, fmod(this->offset, 1.0));
			break;
		}

		case 6: {
			int off = ((int) this->offset) & 0xfc;
			int lfo = (int) (sawOsc(this->time * 8) * 4);

			int i = this->sfx * 68 + PEMSA_RAM_SFX + (int) (off + lfo) * 2;
			int note = (*(this->emulator->getMemoryModule()->ram + i) & 0b00111111);

			this->frequency = NOTE_TO_FREQUENCY(note);
			break;
		}

		case 7: {
			int off = ((int) this->offset) & 0xfc;
			int lfo = (int) (sawOsc(this->time * 4) * 4);

			int i = this->sfx * 68 + PEMSA_RAM_SFX + (int) (off + lfo) * 2;
			int note = (*(this->emulator->getMemoryModule()->ram + i) & 0b00111111);

			this->frequency = NOTE_TO_FREQUENCY(note);
			break;
		}

		case 0: default: break;
	}

	this->offset += 7350.0 / (61 * this->speed * PEMSA_SAMPLE_RATE);
	this->waveOffset += (int) this->frequency / (double) PEMSA_SAMPLE_RATE;
	this->time += 1.0 / PEMSA_SAMPLE_RATE;

	if (this->volume == 0) {
		return 0;
	}

	return pemsa_sample(this->instrument, this->waveOffset) * (vol / 7.0) * 0.5;
}

bool PemsaAudioChannel::isActive() {
	return this->active;
}