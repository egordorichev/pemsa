#ifndef PEMSA_INPUT_MODULE_HPP
#define PEMSA_INPUT_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "pemsa/pemsa_module.hpp"
#include "pemsa/input/pemsa_input_backend.hpp"

class PemsaInputModule : public PemsaModule {
	public:
		PemsaInputModule(PemsaEmulator* emulator, PemsaInputBackend* backend);
		~PemsaInputModule();
	private:
		PemsaInputBackend* backend;
};

#endif