#include "pemsa/pemsa_emulator.hpp"

#include <cmath>

static PemsaEmulator* emulator;

/*
 * todo:
 * add(t, v)
 * all(t)
 * del(t, v)
 * cocreate(func)
 * coresume(cor)
 * costatus(cor)
 * yield()
 * sub(str, from, [to])
 * menuitem(index, [label, callback])
 * extcmd(cmd)
 * stat(n)
 */

static int printh(lua_State* state) {
	int top = lua_gettop(state);

	for (int i = 1; i <= top; i++) {
		lua_pushvalue(state, i);
		const char *str = lua_tostring(state, -1);

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

void pemsa_open_system_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "printh", printh);
	lua_register(state, "time", time);
	lua_register(state, "t", time);

	lua_register(state, "tonum", tonum);
	lua_register(state, "tostr", tostr);
}