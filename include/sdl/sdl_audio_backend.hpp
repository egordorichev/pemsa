#ifndef SDL_AUDIO_BACKEND_HPP
#define SDL_AUDIO_BACKEND_HPP

#include "pemsa/audio/pemsa_audio_backend.hpp"

class SdlAudioBackend : public PemsaAudioBackend {
	public:
		void update() override;
};

#endif