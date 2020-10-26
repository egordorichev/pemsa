#include "pemsa/memory/pemsa_draw_state_module.hpp"
#include "pemsa/util/pemsa_util.hpp"
#include "pemsa/pemsa_emulator.hpp"

PemsaEmulator* emulator;

static int pal(lua_State* state) {
	PemsaDrawStateModule* drawStateModule = emulator->getDrawStateModule();

	if (lua_gettop(state) == 0) {
		for (int i = 0; i < 16; i++) {
			drawStateModule->setDrawColor(i, i);
			drawStateModule->setScreenColor(i, i);
			drawStateModule->setTransparent(i, i == 0);
		}

		return 0;
	}

	int color = luaL_checknumber(state, 1);
	int replacement = luaL_checknumber(state, 2);
	int p = luaL_optnumber(state, 3, 0);

	if (p == 0) {
		drawStateModule->setDrawColor(color, replacement);
	} else {
		drawStateModule->setScreenColor(color, replacement);
	}

	return 0;
}

static int palt(lua_State* state) {
	PemsaDrawStateModule* drawStateModule = emulator->getDrawStateModule();

	if (lua_gettop(state) == 0) {
		for (int i = 0; i < 16; i++) {
			drawStateModule->setTransparent(i, i == 0);
		}

		return 0;
	}

	int color = luaL_checknumber(state, 1);
	bool transparent = pemsa_optional_bool(state, 2, color == 0);

	drawStateModule->setTransparent(color, transparent);
	return 0;
}

void pemsa_open_draw_state_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "palt", palt);
	lua_register(state, "pal", pal);
}