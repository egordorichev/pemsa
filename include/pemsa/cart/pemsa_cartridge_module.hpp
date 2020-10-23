#ifndef PEMSA_CARTRIDGE_MODULE_HPP
#define PEMSA_CARTRIDGE_MODULE_HPP

#include "pemsa/pemsa_module.hpp"

class PemsaCartridgeModule : public PemsaModule {
	public:
		PemsaCartridgeModule(PemsaEmulator* emulator);
		bool load(const char* path);
};

#endif