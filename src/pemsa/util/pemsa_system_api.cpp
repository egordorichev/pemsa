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
 * stat(n)
 */

static int printh(lua_State* state) {
	int top = lua_gettop(state);

	for (int i = 1; i <= top; i++) {
		lua_pushvalue(state, i);
		const char *str;

		if (lua_istable(state, i)) {
			str = "table";
			lua_pop(state, 1);
		} else if (lua_isfunction(state, i)) {
			str = "function";
			lua_pop(state, 1);
		} else if (lua_isnil(state, i)) {
			str = "nil";
			lua_pop(state, 1);
		} else if (lua_isboolean(state, i)) {
			str = lua_toboolean(state, i) ? "true" : "false";
			lua_pop(state, 1);
		} else {
			str = lua_tostring(state, -1);
		}

		if (str) {
			printf("%s\t", str);
		} else {
			printf("%s\t", lua_typename(state, i));
		}

		lua_pop(state, 1);
	}

	lua_pop(state, 1);
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