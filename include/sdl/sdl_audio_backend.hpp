#ifndef SDL_AUDIO_BACKEND_HPP
#define SDL_AUDIO_BACKEND_HPP

#include "pemsa/audio/pemsa_audio_backend.hpp"
#include "SDL_audio.h"

class SdlAudioBackend : public PemsaAudioBackend {
	public:
		~SdlAudioBackend();

		void setupBuffer() override;
	private:
		SDL_AudioDeviceID stream;
};

#endif