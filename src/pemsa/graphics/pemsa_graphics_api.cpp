#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <cmath>
#include <cstring>

/*
 * I'm sorry to myself and whoever is reading this, but this is literally the only way
 * to do this. I can't cast lambdas with captured emulator variable to luaC_Function nor can I capture a method.
 * So this is my last resort - global variable, I'm sorry.
 */

static PemsaEmulator* emulator;

static int flip(lua_State* state) {
	PemsaCartridgeModule* cartridgeModule = emulator->getCartridgeModule();

	std::unique_lock<std::mutex> uniqueLock(*cartridgeModule->getMutex());
	cartridgeModule->getLock()->wait(uniqueLock);

	return 0;
}

static int cls(lua_State* state) {
	// todo: take default color from drawstate
	int color = luaL_optnumber(state, 1, 0);

	memset(emulator->getMemoryModule()->ram + PEMSA_RAM_SCREEN, (color << 4) + color, 0x2000);
	return 0;
}

static int pset(lua_State* state) {
	int x = round(luaL_checknumber(state, 1));
	int y = round(luaL_checknumber(state, 2));

	int c;

	if (lua_gettop(state) == 3) {
		c = luaL_checknumber(state, 3);
	} else {
		// TODO: read from drawstate
		c = 6;
	}

	// TODO: take camera/clip/screen bounds into the account
	emulator->getMemoryModule()->setPixel(x, y, c % 16, PEMSA_RAM_SCREEN);

	// TODO: update drawcolor
	return 0;
}

static int circ(lua_State* state) {
	int ox = round(luaL_checknumber(state, 1));
	int oy = round(luaL_checknumber(state, 2));
	int r = round(luaL_optnumber(state, 3, 1));

	// todo: default from drawstate
	int c = (int) luaL_optnumber(state, 4, 0) % 16;

	int x = r;
	int y = 0;
	int decisionOver2 = 1 - x;

	PemsaMemoryModule* mem = emulator->getMemoryModule();
	
	while (y <= x) {
		mem->setPixel(ox + x, oy + y, c, PEMSA_RAM_SCREEN);
		mem->setPixel(ox + y, oy + x, c, PEMSA_RAM_SCREEN);
		mem->setPixel(ox - x, oy + y, c, PEMSA_RAM_SCREEN);
		mem->setPixel(ox - y, oy + x, c, PEMSA_RAM_SCREEN);

		mem->setPixel(ox - x, oy - y, c, PEMSA_RAM_SCREEN);
		mem->setPixel(ox - y, oy - x, c, PEMSA_RAM_SCREEN);
		mem->setPixel(ox + x, oy - y, c, PEMSA_RAM_SCREEN);
		mem->setPixel(ox + y, oy - x, c, PEMSA_RAM_SCREEN);
		
		y++;

		if (decisionOver2 < 0) {
			decisionOver2 += 2 * y + 1;
		} else {
			x--;
			decisionOver2 += 2 * (y - x) + 1;
		}
	}

	return 0;
}

static inline void horizontalLine(int x0, int y, int x1, int c) {
	PemsaMemoryModule* mem = emulator->getMemoryModule();

	for (int x = x0; x <= x1; x++) {
		mem->setPixel(x, y, c, PEMSA_RAM_SCREEN);
	}
}

static inline void plot(int cx, int cy, int x, int y, int c) {
	horizontalLine(cx - x, cy + y, cx + x, c);

	if (y != 0) {
		horizontalLine(cx - x, cy - y, cx + x, c);
	}
}

static int circfill(lua_State* state) {
	int ox = round(luaL_checknumber(state, 1));
	int oy = round(luaL_checknumber(state, 2));
	int r = round(luaL_optnumber(state, 3, 1));

	// todo: default from drawstate
	int c = (int) luaL_optnumber(state, 4, 0) % 16;

	int x = r;
	int y = 0;
	int error = 1 - r;

	while (y <= x) {
		plot(ox, oy, x, y, c);

		if (error < 0) {
			error += 2 * y + 3;
		} else {
			if (x != y) {
				plot(ox, oy, y, x, c);
			}

			x--;
			error += 2 * (y - x) + 3;
		}

		y++;
	}
}

void pemsa_open_graphics_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "flip", flip);
	lua_register(state, "cls", cls);
	lua_register(state, "pset", pset);
	lua_register(state, "circ", circ);
	lua_register(state, "circfill", circfill);
}