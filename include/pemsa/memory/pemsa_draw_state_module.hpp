#ifndef PEMSA_DRAW_STATE_MODULE_HPP
#define PEMSA_DRAW_STATE_MODULE_HPP

#include "pemsa/pemsa_module.hpp"
#include "pemsa/pemsa_predefines.hpp"

#include <cstdint>

class PemsaDrawStateModule : public PemsaModule {
	public:
		PemsaDrawStateModule(PemsaEmulator* emulator);

		void reset() override;
		bool isTransparent(int color);
		void setTransparent(int color, bool transparent);

		int getScreenColor(int color);
		void setScreenColor(int color, int replacement);
		int getDrawColor(int color);
		void setDrawColor(int color, int replacement);

		int getColor();
		void setColor(int color);

		void setFillPattern(int p);
		int getFillPattern();

		void setFillPatternTransparent(bool transparent);
		bool isFillPatternTransparent();

		bool getFillPatternBit(int x, int y);

		int getCameraX();
		void setCameraX(int x);
		int getCameraY();
		void setCameraY(int y);

		int getCursorX();
		void setCursorX(int x);
		int getCursorY();
		void setCursorY(int y);
};

void pemsa_open_draw_state_api(PemsaEmulator* machine, lua_State* state);

#endif