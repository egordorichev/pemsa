#include "pemsa/pemsa_emulator.hpp"

#include <cmath>
#include <iostream>

static PemsaEmulator* emulator;

/*
 * todo:
 * cocreate(func)
 * coresume(cor)
 * costatus(cor)
 * yield()
 * menuitem(index, [label, callback])
 * extcmd(cmd)
 */

const char* pemsa_to_string(lua_State* state, int i) {
	lua_pushvalue(state, i);
	const char *str = nullptr;

	if (lua_istable(state, -1)) {
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

static int time(lua_State* state) {
	lua_pushnumber(state, emulator->getCartridgeModule()->getCart()->time);
	return 1;
}

static int tonum(lua_State* state) {
	lua_pushnumber(state, lua_tonumber(state, 1));
	return 1;
}

static int tostr(lua_State* state) {
	lua_pushstring(state, lua_tostring(state, 1));
	return 1;
}

static int menuitem(lua_State* state) {
	std::cerr << "Warning: menuitem() is not currently implemented\n";
	return 0;
}

static int stat(lua_State* state) {
	int id = luaL_checknumber(state, 1);
	double result = 0;

	switch (id) {
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
			result = emulator->getInputModule()->getBackend()->getMouseX();
			break;
		}

		case 33: {
			result = emulator->getInputModule()->getBackend()->getMouseY();
			break;
		}

		case 34: {
			result = emulator->getInputModule()->getBackend()->getMouseMask();
			break;
		}

		default: {
			std::cerr << "Warning: stat() is not fully implemented\n";
			break;
		}
	}

	lua_pushnumber(state, result);
	return 1;
}

void pemsa_open_system_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "printh", printh);
	lua_register(state, "time", time);
	lua_register(state, "t", time);

	lua_register(state, "tonum", tonum);
	lua_register(state, "tostr", tostr);

	lua_register(state, "menuitem", menuitem);
	lua_register(state, "stat", stat);
}