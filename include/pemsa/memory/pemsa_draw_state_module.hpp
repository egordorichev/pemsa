#ifndef PEMSA_DRAW_STATE_MODULE_HPP
#define PEMSA_DRAW_STATE_MODULE_HPP

#include "pemsa/pemsa_module.hpp"
#include "pemsa/pemsa_predefines.hpp"

#include <cstdint>

class PemsaDrawStateModule : public PemsaModule {
	public:
		PemsaDrawStateModule(PemsaEmulator* emulator);

		void reset();
		bool isTransparent(int color);
};

#endif