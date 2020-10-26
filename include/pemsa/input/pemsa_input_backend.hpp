#ifndef PEMSA_INPUT_BACKEND_HPP
#define PEMSA_INPUT_BACKEND_HPP

#include "pemsa/pemsa_backend.hpp"

class PemsaInputBackend : public PemsaBackend {
	public:
		virtual bool isButtonDown(int i, int p) = 0;
		virtual bool isButtonPressed(int i, int p) = 0;
		virtual void update() = 0;
};

#endif