#include "pemsa/pemsa_emulator.hpp"

#include <ctime>
#include <iostream>

static PemsaEmulator* emulator;

const char* pemsa_to_string(lua_State* state, int i) {
	lua_pushvalue(state, i);
	const char *str = nullptr;

	if (lua_isnil(state, -1)) {
		str = "nil";
	} else if (lua_istable(state, -1)) {
		str = "table";
	} else if (lua_isfunction(state, -1)) {
		str = "function";
	} else if (lua_isnil(state, -1)) {
		str = "nil";
	} else if (lua_isboolean(state, -1)) {
		str = lua_toboolean(state, -1) ? "true" : "false";
	} else {
		str = lua_tostring(state, -1);
	}

	if (!str) {
		str = lua_typename(state, -1);
	}

	lua_pop(state, 1);
	return str;
}

static int printh(lua_State* state) {
	int top = lua_gettop(state);

	for (int i = 1; i <= top; i++) {
		printf("%s\t", pemsa_to_string(state, i));
	}

	printf("\n");
	return 0;
}

static int pemsa_time(lua_State* state) {
	pemsa_pushnumber(state, emulator->getCartridgeModule()->getCart()->time);
	return 1;
}

static int tonum(lua_State* state) {
	if (lua_gettop(state) < 1 || !lua_isstring(state, 1)) {
		return 0;
	}

	const char* ostr = luaL_checkstring(state, 1);
	char *endptr;

	pemsa_pushnumber_raw(state, strtofix16(ostr, &endptr));

	return 1;
}

static int tostr(lua_State* state) {
	if (pemsa_optional_bool(state, 2, false)) {
		fix16_t num = pemsa_checknumber_raw(state, 1);
		char buff[50];

		fix16_to_strx(num, buff);
		lua_pushstring(state, buff);

		return 1;
	}

	lua_pushstring(state, lua_tostring(state, 1));
	return 1;
}

static int extcmd(lua_State* state) {
	std::cerr << "Warning: extcmd() is not currently implemented\n";
	return 0;
}

static int stop(lua_State* state) {
	emulator->getCartridgeModule()->stop();
	luaL_error(state, "the cart was stopped");

	return 0;
}

static int exit(lua_State* state) {
	emulator->stop();
	return 0;
}

static int stat(lua_State* state) {
	int id = pemsa_checknumber(state, 1);
	double result = 0;

	switch (id) {
		case 0: {
			result = (uint64_t) (((lua_gc(state, LUA_GCCOUNT, 0) << 10) + lua_gc(state, LUA_GCCOUNTB, 0)) / 1024);
			break;
		}

		case 1: {
			result = 0.42;
			break;
		}

		case 4: {
			const char* clipboard = emulator->getInputModule()->getBackend()->getClipboardText();

			if (clipboard == nullptr) {
				lua_pushnil(state);
			} else {
				lua_pushstring(state, clipboard);
			}

			return 1;
		}

		case 5: {
			lua_pushstring(state, "0.2.1b");
			return 1;
		}

		case 7: {
			result = emulator->getGraphicsModule()->getBackend()->getFps();
			break;
		}

		case 8: {
			result = emulator->getCartridgeModule()->getCart()->highFps ? 60 : 30;
			break;
		}

		case 16:
		case 17:
		case 18:
		case 19: {
			PemsaAudioChannel* channel = emulator->getAudioModule()->getChannel(id - 16);
			result = channel->isActive() ? channel->getSfx() : -1;

			break;
		}

		case 20:
		case 21:
		case 22:
		case 23: {
			PemsaAudioChannel* channel = emulator->getAudioModule()->getChannel(id - 20);
			result = channel->isActive() ? channel->getNote() : -1;

			break;
		}

		case 24: {
			result = emulator->getAudioModule()->getMusic();
			break;
		}

		case 26: {
			result = emulator->getAudioModule()->getOffset();
			break;
		}

		case 30: {
			lua_pushboolean(state, emulator->getInputModule()->getBackend()->hasKey());
			return 1;
		}

		case 31: {
			const char* key = emulator->getInputModule()->getBackend()->readKey();

			if (key == nullptr) {
				lua_pushnil(state);
			} else {
				lua_pushstring(state, key);
			}

			return 1;
		}

		case 32: {
			if (emulator->getMemoryModule()->ram[0x5f2d] != 0) {
				result = emulator->getInputModule()->getBackend()->getMouseX();
			}

			break;
		}

		case 33: {
			if (emulator->getMemoryModule()->ram[0x5f2d] != 0) {
				result = emulator->getInputModule()->getBackend()->getMouseY();
			}

			break;
		}

		case 34: {
			if (emulator->getMemoryModule()->ram[0x5f2d] != 0) {
				result = emulator->getInputModule()->getBackend()->getMouseMask();
			}

			break;
		}

		case 80: {
			time_t t = time(NULL);
			result = gmtime(&t)->tm_year;

			break;
		}

		case 81: {
			time_t t = time(NULL);
			result = gmtime(&t)->tm_mon;

			break;
		}

		case 82: {
			time_t t = time(NULL);
			result = gmtime(&t)->tm_mday;

			break;
		}

		case 83: {
			time_t t = time(NULL);
			result = gmtime(&t)->tm_hour;

			break;
		}

		case 84: {
			time_t t = time(NULL);
			result = gmtime(&t)->tm_min;

			break;
		}

		case 85: {
			time_t t = time(NULL);
			result = gmtime(&t)->tm_sec;

			break;
		}

		case 90: {
			time_t t = time(NULL);
			result = localtime(&t)->tm_year;

			break;
		}

		case 91: {
			time_t t = time(NULL);
			result = localtime(&t)->tm_mon;

			break;
		}

		case 92: {
			time_t t = time(NULL);
			result = localtime(&t)->tm_mday;

			break;
		}

		case 93: {
			time_t t = time(NULL);
			result = localtime(&t)->tm_hour;

			break;
		}

		case 94: {
			time_t t = time(NULL);
			result = localtime(&t)->tm_min;

			break;
		}

		case 95: {
			time_t t = time(NULL);
			result = localtime(&t)->tm_sec;

			break;
		}

		case 102: {
			result = 0;
			break;
		}

		default: {
			std::cerr << "Warning: stat() is not fully implemented, state " << id << " is not implemented\n";
			break;
		}
	}

	pemsa_pushnumber(state, result);
	return 1;
}

static int next(lua_State *state) {
	if (lua_isnil(state, 1)) {
		lua_pushnil(state);
		return 1;
	}

	luaL_checktype(state, 1, LUA_TTABLE);
	lua_settop(state, 2);

	if (lua_next(state, 1)) {
		return 2;
	} else {
		lua_pushnil(state);
		return 1;
	}
}


static int pairs(lua_State *state) {
	luaL_checkany(state, 1);

	if (luaL_getmetafield(state, 1, "__pairs") == LUA_TNIL) {
		lua_pushcfunction(state, next);
		lua_pushvalue(state, 1);
		lua_pushnil(state);
	} else {
		lua_pushvalue(state, 1);
		lua_call(state, 1, 3);
	}

	return 3;
}

void pemsa_open_system_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "printh", printh);
	lua_register(state, "time", pemsa_time);
	lua_register(state, "t", pemsa_time);

	lua_register(state, "tonum", tonum);
	lua_register(state, "tostr", tostr);

	lua_register(state, "extcmd", extcmd);
	lua_register(state, "stop", stop);
	lua_register(state, "exit", exit);
	lua_register(state, "stat", stat);
	lua_register(state, "pairs", pairs);
}