#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

static PemsaEmulator* emulator;

static int fget(lua_State* state) {
	int tile = luaL_checknumber(state, 1);

	if (tile < 0 || tile > 255) {
		lua_pushnumber(state, 0);
		return 1;
	}

	int mask = emulator->getMemoryModule()->ram[PEMSA_RAM_GFX_PROPS + tile];

	if (lua_gettop(state) == 2) {
		int bit = luaL_checkinteger(state, 2);
		lua_pushnumber(state, mask & (1 << bit));
	} else {
		lua_pushnumber(state, mask);
	}

	return 1;
}

static int fset(lua_State* state) {
	int tile = luaL_checknumber(state, 1);

	if (tile < 0 || tile > 255) {
		return 0;
	}

	uint8_t* ram = emulator->getMemoryModule()->ram + PEMSA_RAM_GFX_PROPS + tile;

	int bit = luaL_checknumber(state, 2);
	bool on = pemsa_optional_bool(state, 3, false);

	*ram = SET_BIT(*ram, bit, on)
	return 0;
}

static int mget(lua_State* state) {
	int x = luaL_checknumber(state, 1);
	int y = luaL_checknumber(state, 2);

	if (x < 0 || y < 0 || x > 127 || y > 63) {
		lua_pushnumber(state, 0);
	} else {
		lua_pushnumber(state, emulator->getMemoryModule()->ram[PEMSA_RAM_MAP + x + y * 128]);
	}

	return 1;
}

static int mset(lua_State* state) {
	int x = luaL_checknumber(state, 1);
	int y = luaL_checknumber(state, 2);
	int tile = luaL_checknumber(state, 3);

	if (!(x < 0 || y < 0 || x > 127 || y > 63)) {
		emulator->getMemoryModule()->ram[PEMSA_RAM_MAP + x + y * 128] = tile % 256;
	}

	return 0;
}

void pemsa_open_memory_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "fget", fget);
	lua_register(state, "fset", fset);
	lua_register(state, "mget", mget);
	lua_register(state, "mset", mset);
}