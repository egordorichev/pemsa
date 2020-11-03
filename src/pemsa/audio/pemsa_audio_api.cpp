#include "pemsa/pemsa_emulator.hpp"

static PemsaEmulator* emulator;

static int music(lua_State* state) {
	int n = luaL_checknumber(state, 1);

	if (n >= 0 && n < 64) {
		emulator->getAudioModule()->playMusic(n);
	}

	return 0;
}

static int sfx(lua_State* state) {
	int n = luaL_checknumber(state, 1);

	if (n >= -1 && n < 64) {
		emulator->getAudioModule()->playSfx(n, luaL_optnumber(state, 2, -1));
	}

	return 0;
}

void pemsa_open_audio_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "music", music);
	lua_register(state, "sfx", sfx);
}