#ifndef PEMSA_AUDIO_MODULE_HPP
#define PEMSA_AUDIO_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "pemsa/pemsa_module.hpp"
#include "pemsa/audio/pemsa_audio_backend.hpp"

class PemsaAudioModule : public PemsaModule {
	public:
		PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend);
		~PemsaAudioModule();
	private:
		PemsaAudioBackend* backend;
};

#endif