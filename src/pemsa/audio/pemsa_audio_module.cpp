#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

PemsaAudioModule::PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		this->channels[i] = new PemsaAudioChannel(emulator, i);
	}

	this->backend->setEmulator(emulator);

	this->time = 0;
	this->musicOffset = 0;
	this->currentMusic = -1;
	this->paused = false;

	this->backend->setupBuffer();
}

PemsaAudioModule::~PemsaAudioModule() {
	delete this->backend;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		delete this->channels[i];
	}
}

double PemsaAudioModule::sample() {
	if (paused) {
		return 0;
	}

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
			} else {
				bool found = false;

				if (this->currentMusic < 63) {
					uint8_t *songRam = ram + (this->currentMusic + 1) * 4 + PEMSA_RAM_SONG;

					for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
						int data = songRam[i];

						if (data != 0 && !IS_BIT_SET(data, 6)) {
							found = true;
							break;
						}
					}
				}

				bool loopFound = false;

				if (!found) {
					for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
						if (ram[this->channels[i]->getSfx() * 68 + PEMSA_RAM_SFX + 67] != 0) {
							loopFound = true;
							break;
						}
					}

					if (!loopFound) {
						this->playMusic(-1);
					}
				}

				if (found || loopFound) {
					this->playMusic(this->currentMusic + (found ? 1 : 0));
				}
			}
		}
	}

	double result = 0;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		result += this->channels[i]->sample();
	}

	return result / PEMSA_CHANNEL_COUNT;
}

void PemsaAudioModule::playSfx(int sfx, int ch, int offset, int length) {
	if (ch == -2) {
		for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
			if (this->channels[i]->getSfx() == sfx) {
				this->channels[i]->stop();
			}
		}

		return;
	}

	if (ch > -1 && ch < PEMSA_CHANNEL_COUNT) {
		if (sfx == -1) {
			this->channels[ch]->stop();
		} else {
			this->channels[ch]->play(sfx, false, offset, length);
		}

		return;
	}

	if (sfx == -1) {
		return;
	}

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		PemsaAudioChannel* channel = this->channels[i];

		if (!channel->isActive()) {
			channel->play(sfx, false, offset, length);
			return;
		}
	}

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		PemsaAudioChannel* channel = this->channels[i];

		if (!channel->isPlayingMusic()) {
			channel->play(sfx, false, offset, length);
			return;
		}
	}
}

void PemsaAudioModule::playMusic(int music) {
	uint8_t* ram = emulator->getMemoryModule()->ram;

	if (this->currentMusic != -1) {
		for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
			PemsaAudioChannel *channel = this->channels[i];

			if (channel->isPlayingMusic()) {
				channel->stop();
			}
		}
	}

	this->currentMusic = music;

	if (music == -1) {
		return;
	}

	this->musicSpeed = 1;
	this->time = 0;
	this->musicOffset = 0;

	uint8_t* songRam = ram + this->currentMusic * 4 + PEMSA_RAM_SONG;

	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		int sfxData = songRam[i];

		if (!IS_BIT_SET(sfxData, 6)) {
			int sfx = sfxData % 64;

			this->channels[i]->play(sfx, true, 0, 32);
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

void PemsaAudioModule::reset() {
	for (int i = 0; i < PEMSA_CHANNEL_COUNT; i++) {
		this->channels[i]->stop();
	}

	this->paused = false;
}

void PemsaAudioModule::setPaused(bool paused) {
	this->paused = paused;
}