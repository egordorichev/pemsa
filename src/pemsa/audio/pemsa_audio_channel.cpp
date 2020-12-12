#include "pemsa/audio/pemsa_audio_channel.hpp"
#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/audio/pemsa_wave_functions.hpp"
#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

static inline double lerp(double a, double b, double t) {
	return (b - a) * t + a;
}

static inline double osc(double x) {
	return (fabs(fmod(x, 1) * 2 - 1) * 2 - 1) * 0.7;
}

static inline double sawOsc(double x) {
	return fmod(x, 1);
}

PemsaAudioChannel::PemsaAudioChannel(PemsaEmulator* emulator, int channelId) {
	this->emulator = emulator;
	this->channelId = channelId;
}

void PemsaAudioChannel::play(int sfx, bool music, int offset, int length) {
	this->playingMusic = music;
	this->stopLooping = false;
	this->length = length;

	PemsaChannelInfo* info = &this->infos[0];

	info->note = 0;
	info->lastNote = 0;
	info->sfx = sfx;
	info->offset = offset;
	info->lastStep = offset - 1;
	info->active = true;
	info->speed = fmax(1, this->emulator->getMemoryModule()->ram[sfx * 68 + PEMSA_RAM_SFX + 65]);
}

double PemsaAudioChannel::sample() {
	if (!this->infos[0].active) {
		return 0;
	}

	return this->prepareSample(0);
}

bool PemsaAudioChannel::isActive() {
	return this->infos[0].active;
}

void PemsaAudioChannel::stop() {
	this->playingMusic = false;
	this->infos[0].active = false;
}

int PemsaAudioChannel::getSfx() {
	return this->infos[0].sfx;
}

int PemsaAudioChannel::getNote() {
	return this->infos[0].note;
}

double PemsaAudioChannel::applyFx(int id, int fx) {
	PemsaChannelInfo* info = &this->infos[id];
	double vol = info->volume;

	switch (fx) {
		case 1: {
			info->frequency = lerp(NOTE_TO_FREQUENCY(info->lastNote), NOTE_TO_FREQUENCY(info->note), fmod(info->offset, 1.0));
			break;
		}

		case 2: {
			info->frequency = lerp(NOTE_TO_FREQUENCY(info->note), NOTE_TO_FREQUENCY(info->note + 0.5), osc(info->offset));
			break;
		}

		case 3: {
			info->frequency = lerp(NOTE_TO_FREQUENCY(info->note), 0, fmod(info->offset, 1.0));
			break;
		}

		case 4: {
			vol = lerp(0, (double) info->volume, fmod(info->offset, 1.0));
			break;
		}

		case 5: {
			vol = lerp((double) info->volume, 0, fmod(info->offset, 1.0));
			break;
		}

		case 6: {
			int off = ((int) info->offset) & 0xfc;
			int lfo = (int) (sawOsc(info->offset) * 4);

			int i = info->sfx * 68 + PEMSA_RAM_SFX + (int) (off + lfo) * 2;
			int note = (*(this->emulator->getMemoryModule()->ram + i) & 0b00111111);

			info->frequency = NOTE_TO_FREQUENCY(note);
			break;
		}

		case 7: {
			int off = ((int) info->offset) & 0xfc;
			int lfo = (int) (sawOsc(info->offset * 0.5) * 4);

			int i = info->sfx * 68 + PEMSA_RAM_SFX + (int) (off + lfo) * 2;
			int note = (*(this->emulator->getMemoryModule()->ram + i) & 0b00111111);

			info->frequency = NOTE_TO_FREQUENCY(note);
			break;
		}

		case 0: default: break;
	}

	return vol;
}

double PemsaAudioChannel::sampleAt(int id) {
	PemsaChannelInfo* info = &this->infos[id];
	return this->adjustVolume(id, pemsa_sample(this->channelId, info->instrument, info->waveOffset), applyFx(id, info->fx) / 7.0);
}

static int min(int a, int b) {
	return a < b ? a : b;
}

double PemsaAudioChannel::prepareSample(int id) {
	PemsaChannelInfo* info = &this->infos[id];

	if ((int) info->offset > info->lastStep) {
		uint8_t* ram = emulator->getMemoryModule()->ram;
		info->lastNote = info->note;

		int loopEnd = min(ram[info->sfx * 68 + PEMSA_RAM_SFX + 67], this->length);

		if (loopEnd != 0 && !this->stopLooping) {
			if (info->offset >= loopEnd) {
				info->offset = ram[info->sfx * 68 + PEMSA_RAM_SFX + 66];
				info->lastStep = (int) info->offset - 1;

				if (id == 0) {
					PemsaChannelInfo* secondInfo = &this->infos[1];

					secondInfo->offset = 0;
					secondInfo->lastStep = -1;
				}
			} else {
				info->lastStep = (int) info->offset;
			}
		} else if (info->offset >= this->length) {
			this->stopLooping = false;
			info->active = false;

			info->offset = 0;
			info->lastStep = -1;

			if (id == 0) {
				PemsaChannelInfo* secondInfo = &this->infos[1];

				secondInfo->offset = 0;
				secondInfo->lastStep = 0;
			}

			return 0;
		} else {
			info->lastStep = (int) info->offset;
		}

		int i = info->sfx * 68 + PEMSA_RAM_SFX + (int) info->offset * 2;

		uint8_t lo = ram[i];
		uint8_t hi = ram[i + 1];

		info->note = (lo & 0b00111111);

		info->instrument = (uint8_t) (((lo & 0b11000000) >> 6) | ((hi & 0b1) << 2));
		info->volume = (uint8_t) ((hi & 0b00001110) >> 1);
		info->fx = (uint8_t) ((hi & 0b01110000) >> 4);
		info->isCustom = (uint8_t) ((hi & 0b10000000) >> 7) == 1;

		if (id == 0 && info->isCustom) {
			PemsaChannelInfo* secondInfo = &this->infos[1];

			secondInfo->sfx = info->instrument;
			secondInfo->active = true;
			secondInfo->speed = fmax(1, this->emulator->getMemoryModule()->ram[secondInfo->sfx * 68 + PEMSA_RAM_SFX + 65]);
		}

		info->frequency = NOTE_TO_FREQUENCY(info->note);
	}

	info->offset += 7350.0 / (61 * info->speed * PEMSA_SAMPLE_RATE);
	info->waveOffset += (double) info->frequency / (double) PEMSA_SAMPLE_RATE;

	if (id == 1) {
		PemsaChannelInfo* secondInfo = &this->infos[0];
		double vol = applyFx(0, secondInfo->fx);

		info->note = (this->emulator->getMemoryModule()->ram[info->sfx * 68 + PEMSA_RAM_SFX + (int) info->offset * 2] & 0b00111111) + (secondInfo->note - 2);

		return this->adjustVolume(id, pemsa_sample(this->channelId, info->instrument, info->waveOffset), (vol / 7.0) * (applyFx(id, info->fx) / 7.0));
	} else if (id == 0 && info->isCustom) {
		return this->prepareSample(1);
	}

	return this->sampleAt(id);
}

double PemsaAudioChannel::adjustVolume(int id, double wave, double volume) {
	PemsaChannelInfo* info = &this->infos[id];
	info->lastVolume += (volume - info->lastVolume) * 0.05;

	return wave * info->lastVolume;
}

bool PemsaAudioChannel::isPlayingMusic() {
	return this->playingMusic;
}

void PemsaAudioChannel::preventLoop() {
	this->stopLooping = true;
}

PemsaChannelInfo::PemsaChannelInfo() {
	this->active = false;
	this->lastStep = -1;
	this->waveOffset = 0;
	this->offset = 0;
	this->lastVolume = 0;
}