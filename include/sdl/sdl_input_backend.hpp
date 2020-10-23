#ifndef SDL_INPUT_BACKEND_HPP
#define SDL_INPUT_BACKEND_HPP

#include "pemsa/input/pemsa_input_backend.hpp"

class SdlInputBackend : public PemsaInputBackend {
	public:
		bool isButtonDown(int i, int p) override;
};

#endif