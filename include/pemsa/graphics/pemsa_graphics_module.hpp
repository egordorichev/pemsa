#ifndef PEMSA_GRAPHICS_MODULE_HPP
#define PEMSA_GRAPHICS_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "pemsa/pemsa_module.hpp"
#include "pemsa/graphics/pemsa_graphics_backend.hpp"

class PemsaGraphicsModule : public PemsaModule {
	public:
		PemsaGraphicsModule(PemsaEmulator* emulator, PemsaGraphicsBackend* backend);
		~PemsaGraphicsModule();
	private:
		PemsaGraphicsBackend* backend;
};

#endif