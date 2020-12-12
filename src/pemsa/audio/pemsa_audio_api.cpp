#include "pemsa/pemsa_emulator.hpp"

static PemsaEmulator* emulator;

static int music(lua_State* state) {
	int n = pemsa_checknumber(state, 1);

	if (n >= -1 && n < 64) {
		emulator->getAudioModule()->playMusic(n);
	}

	return 0;
}

static int sfx(lua_State* state) {
	int n = pemsa_checknumber(state, 1);
	int channel = pemsa_optional_number(state, 2, -1);
	int offset = pemsa_optional_number(state, 3, 0);
	int length = pemsa_optional_number(state, 4, 31);

	PemsaAudioModule* audioModule = emulator->getAudioModule();

	if (n == -1) {
		if (channel >= 0 && channel < PEMSA_CHANNEL_COUNT) {
			audioModule->getChannel(channel)->stop();
		}
	} else if (n == -2) {
		if (channel >= 0 && channel < PEMSA_CHANNEL_COUNT) {
			audioModule->getChannel(channel)->preventLoop();
		}
	} else {
		if (n >= 0 && n < 64) {
			audioModule->playSfx(n, channel, offset, length);
		}
	}

	return 0;
}

static int set_audio_paused(lua_State* state) {
	emulator->getAudioModule()->setPaused(pemsa_optional_bool(state, 1, false));
	return 0;
}

void pemsa_open_audio_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "music", music);
	lua_register(state, "sfx", sfx);
	lua_register(state, "__set_audio_paused", set_audio_paused);
}