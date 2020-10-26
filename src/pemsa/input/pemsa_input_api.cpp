#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

static PemsaEmulator* emulator;

int btn(lua_State* state) {
	int button = luaL_checknumber(state, 1);
	int player = luaL_optnumber(state, 2, 0);

	lua_pushboolean(state, emulator->getInputModule()->isDown(button, player));
	return 1;
}

void pemsa_open_input_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;
	lua_register(state, "btn", btn);
}