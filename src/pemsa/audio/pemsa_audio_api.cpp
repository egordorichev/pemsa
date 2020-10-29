#include "pemsa/pemsa_emulator.hpp"
#include <iostream>

static PemsaEmulator* emulator;

static int music(lua_State* state) {
	std::cerr << "Warning: music() is not currently implemented\n";
	return 0;
}

static int sfx(lua_State* state) {
	std::cerr << "Warning: sfx() is not currently implemented\n";
	return 0;
}

void pemsa_open_audio_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "music", music);
	lua_register(state, "sfx", sfx);
}