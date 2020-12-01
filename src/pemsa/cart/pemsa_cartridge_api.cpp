#include "pemsa/pemsa_emulator.hpp"

static PemsaEmulator* emulator;

static int dget(lua_State* state) {
	int index = pemsa_checknumber(state, 1);

	if (index >= 0 && index < PEMSA_CART_DATA_SIZE) {
		pemsa_pushnumber_raw(state, emulator->getCartridgeModule()->getCart()->cartData[index]);
	} else {
		pemsa_pushnumber(state, 0);
	}

	return 1;
}

static int dset(lua_State* state) {
	int index = pemsa_checknumber(state, 1);

	if (index >= 0 && index < PEMSA_CART_DATA_SIZE) {
		PemsaCartridgeModule* cartridgeModule = emulator->getCartridgeModule();

		cartridgeModule->getCart()->cartData[index] = pemsa_checknumber_raw(state, 2);
		cartridgeModule->saveData();
	}

	return 0;
}

static int cartdata(lua_State* state) {
	const char* name = luaL_checkstring(state, 1);
	emulator->getCartridgeModule()->loadData(name);

	return 0;
}

static int set_paused(lua_State* state) {
	emulator->getCartridgeModule()->setPaused(pemsa_optional_bool(state, 1, false));
	return 0;
}

static int reset(lua_State* state) {
	emulator->getCartridgeModule()->initiateSelfDestruct();
	return 0;
}

void pemsa_open_cartridge_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "dget", dget);
	lua_register(state, "dset", dset);
	lua_register(state, "cartdata", cartdata);
	lua_register(state, "__set_paused", set_paused);
	lua_register(state, "__reset", reset);
}