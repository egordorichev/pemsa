#ifndef PEMSA_AUDIO_CHANNEL_HPP
#define PEMSA_AUDIO_CHANNEL_HPP

#include "pemsa/pemsa_predefines.hpp"
#include <cstdint>

class PemsaAudioChannel {
	public:
		PemsaAudioChannel(PemsaEmulator* emulator);

		void play(int sfx);
		double sample();
		bool isActive();
		void stop();

		int getSfx();
	private:
		PemsaEmulator* emulator;
		bool active;

		int sfx;

		double offset;
		double lastStep;
		double waveOffset;
		double frequency;

		uint8_t speed;
		uint8_t instrument;
		uint8_t volume;
		uint8_t fx;

		int note;
		int lastNote;
		bool isCustom;
};

#endif