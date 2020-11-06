#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

PemsaAudioModule::PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		this->channels[i] = new PemsaAudioChannel(emulator);
	}

	this->backend->setEmulator(emulator);
	this->backend->setupBuffer();

	this->time = 0;
	this->musicOffset = 0;
	this->currentMusic = -1;
}

PemsaAudioModule::~PemsaAudioModule() {
	delete this->backend;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		delete this->channels[i];
	}
}

double PemsaAudioModule::sample() {
	if (this->currentMusic > -1) {
		this->musicOffset += 7350 / (61.0 * this->musicSpeed * PEMSA_SAMPLE_RATE);

		if (this->musicOffset >= 32) {
			uint8_t* ram = emulator->getMemoryModule()->ram;

			bool loopEnd = IS_BIT_SET(ram[PEMSA_RAM_SONG + this->currentMusic * 4 + 1], 7);
			bool halt = IS_BIT_SET(ram[PEMSA_RAM_SONG + this->currentMusic * 4 + 2], 7);

			if (halt) {
				this->playMusic(-1);
			} else if (loopEnd) {
				int music = this->currentMusic - 1;

				while (true) {
					if (IS_BIT_SET(ram[PEMSA_RAM_SONG + music * 4], 7)) {
						this->playMusic(music);
						break;
					}

					music--;

					if (music < 0) {
						break;
					}
				}
			} else if (this->currentMusic < 63) {
				this->playMusic(this->currentMusic + 1);
			}
		}
	}

	double result = 0;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		result += this->channels[i]->sample();
	}

	return result / PEMSA_CHANNEL_COUNT;
}

void PemsaAudioModule::playSfx(int sfx, int ch) {
	if (ch > -1 && ch < PEMSA_CHANNEL_COUNT) {
		if (ch == -1) {
			for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
				if (this->channels[i]->getSfx() == sfx) {
					this->channels[i]->stop();
				}
			}
		} else {
			if (sfx == -1) {
				this->channels[ch]->stop();
			} else {
				this->channels[ch]->play(sfx);
			}
		}

		return;
	}

	if (sfx == -1) {
		return;
	}

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		PemsaAudioChannel* channel = this->channels[i];

		if (!channel->isActive()) {
			channel->play(sfx);
			return;
		}
	}

	if (this->currentMusic == -1) {
		return;
	}

	uint8_t* ram = emulator->getMemoryModule()->ram;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		PemsaAudioChannel* channel = this->channels[i];

		if (channel->getSfx() != ram[PEMSA_RAM_SONG + this->currentMusic * 4 + i]) {
			channel->play(sfx);
			return;
		}
	}
}

void PemsaAudioModule::playMusic(int music) {
	uint8_t* ram = emulator->getMemoryModule()->ram;

	if (this->currentMusic != -1) {
		for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
			PemsaAudioChannel *channel = this->channels[i];

			if (channel->getSfx() == ram[PEMSA_RAM_SONG + this->currentMusic * 4 + i]) {
				channel->stop();
			}
		}
	}

	if (music == -1) {
		if (this->currentMusic != -1) {
			for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
				this->channels[i]->stop();
			}

			this->currentMusic = -1;
		}

		return;
	}

	this->currentMusic = music;
	this->musicSpeed = 1;
	this->time = 0;
	this->musicOffset = 0;

	uint8_t* songRam = ram + this->currentMusic * 4 + PEMSA_RAM_SONG;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		int sfxData = songRam[i];

		if (!IS_BIT_SET(sfxData, 6)) {
			int sfx = sfxData % 64;

			this->channels[i]->play(sfx);
			int speed = fmax(1, ram[PEMSA_RAM_SFX + sfx * 68 + 65]);

			if (speed > this->musicSpeed) {
				this->musicSpeed = speed;
			}
		}
	}
}

PemsaAudioChannel *PemsaAudioModule::getChannel(int i) {
	return this->channels[i];
}

int PemsaAudioModule::getMusic() {
	return this->currentMusic;
}

int PemsaAudioModule::getOffset() {
	return this->musicOffset;
}