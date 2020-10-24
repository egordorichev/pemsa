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

void pemsa_open_graphics_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "flip", flip);
	lua_register(state, "cls", cls);
	lua_register(state, "pset", pset);
}