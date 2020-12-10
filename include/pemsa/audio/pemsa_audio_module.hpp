#ifndef PEMSA_AUDIO_MODULE_HPP
#define PEMSA_AUDIO_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "pemsa/pemsa_module.hpp"
#include "pemsa/audio/pemsa_audio_backend.hpp"
#include "pemsa/audio/pemsa_audio_channel.hpp"

#include <cstdint>

#define PEMSA_SAMPLE_SIZE 2048
#define PEMSA_SAMPLE_RATE 44100
#define PEMSA_CHANNEL_COUNT 4

class PemsaAudioModule : public PemsaModule {
	public:
		PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend);
		~PemsaAudioModule();

		double sample();
		void playSfx(int sfx, int channel = -1, int offset = 0, int length = 31);
		void playMusic(int music);

		PemsaAudioChannel* getChannel(int i);

		int getMusic();
		int getOffset();

		void reset() override;
		void setPaused(bool paused);
	private:
		PemsaAudioBackend* backend;
		PemsaAudioChannel* channels[PEMSA_CHANNEL_COUNT];

		double time;
		double musicOffset;
		int currentMusic = -1;
		int musicSpeed;
		bool paused;
};

void pemsa_open_audio_api(PemsaEmulator* machine, lua_State* state);

#endif