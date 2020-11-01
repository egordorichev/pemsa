#ifndef PEMSA_AUDIO_MODULE_HPP
#define PEMSA_AUDIO_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "pemsa/pemsa_module.hpp"
#include "pemsa/audio/pemsa_audio_backend.hpp"

#include <cstdint>

#define PEMSA_SAMPLE_SIZE 1024
#define PEMSA_SAMPLE_RATE 44100
#define PEMSA_MIN_BYTES_IN_AUDIO PEMSA_SAMPLE_SIZE * 3

class PemsaAudioModule : public PemsaModule {
	public:
		PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend);
		~PemsaAudioModule();

		void update(double dt) override;

		int16_t* getBuffer();
	private:
		PemsaAudioBackend* backend;
		int16_t buffer[PEMSA_SAMPLE_SIZE];
		double time;
};

void pemsa_open_audio_api(PemsaEmulator* machine, lua_State* state);

#endif