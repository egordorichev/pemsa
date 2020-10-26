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
		void setTransparent(int color, bool transparent);

		int getScreenColor(int color);
		void setScreenColor(int color, int replacement);
		int getDrawColor(int color);
		void setDrawColor(int color, int replacement);

		int getColor();
		void setColor(int color);
};

void pemsa_open_draw_state_api(PemsaEmulator* machine, lua_State* state);

#endif