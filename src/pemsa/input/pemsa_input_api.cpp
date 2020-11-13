#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

static PemsaEmulator* emulator;

int btn(lua_State* state) {
	PemsaInputModule* input = emulator->getInputModule();

	if (lua_gettop(state) == 0) {
		int mask = 0;

		for (int i = 0; i < PEMSA_BUTTON_COUNT * 2; i++) {
			SET_BIT(mask, i, input->isDown(i % PEMSA_BUTTON_COUNT, i / PEMSA_BUTTON_COUNT));
		}

		pemsa_pushnumber(state, mask);
		return 1;
	}

	int button = pemsa_checknumber(state, 1);
	int player = pemsa_optnumber(state, 2, 0);

	lua_pushboolean(state, input->isDown(button, player));
	return 1;
}

int btnp(lua_State* state) {
	PemsaInputModule* input = emulator->getInputModule();

	if (lua_gettop(state) == 0) {
		int mask = 0;

		for (int i = 0; i < PEMSA_BUTTON_COUNT * 2; i++) {
			SET_BIT(mask, i, input->isPressed(i % PEMSA_BUTTON_COUNT, i / PEMSA_BUTTON_COUNT));
		}

		pemsa_pushnumber(state, mask);
		return 1;
	}

	int button = pemsa_checknumber(state, 1);
	int player = pemsa_optnumber(state, 2, 0);

	lua_pushboolean(state, input->isPressed(button, player));
	return 1;
}

void pemsa_open_input_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "btn", btn);
	lua_register(state, "btnp", btnp);
}