#ifndef PEMSA_AUDIO_CHANNEL_HPP
#define PEMSA_AUDIO_CHANNEL_HPP

#include "pemsa/pemsa_predefines.hpp"
#include <cstdint>

class PemsaChannelInfo {
	public:
		PemsaChannelInfo();

		bool active;

		int sfx;
		int lastStep;

		double offset;
		double waveOffset;
		double frequency;
		double lastVolume;

		uint8_t speed;
		uint8_t instrument;
		uint8_t volume;
		uint8_t fx;

		int note;
		int lastNote;
		bool isCustom;
};

class PemsaAudioChannel {
	public:
		PemsaAudioChannel(PemsaEmulator* emulator, int channelId);

		void play(int sfx, bool music, int offset, int length);
		double sample();
		bool isActive();
		void stop();

		int getSfx();
		int getNote();

		bool isPlayingMusic();

		void preventLoop();
	private:
		PemsaEmulator* emulator;
		PemsaChannelInfo infos[2];

		double prepareSample(int id);
		double sampleAt(int id);

		double applyFx(int id, int fx);
		double adjustVolume(int id, double wave, double volume);

		int channelId;
		bool playingMusic;
		bool stopLooping;
		int length;
};

#endif