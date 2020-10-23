#ifndef PESMA_BACKEND_HPP
#define PESMA_BACKEND_HPP

#include "pemsa/pemsa_module.hpp"

class PemsaBackend {
	public:
		void setEmulator(PemsaEmulator* emulator);
	protected:
		PemsaEmulator* emulator;
};

#endif