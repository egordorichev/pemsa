#include "pemsa/util/pemsa_util.hpp"

int pemsa_trace_lua(lua_State* state) {
	lua_getfield(state, LUA_GLOBALSINDEX, "debug");
	lua_getfield(state, -1, "traceback");
	lua_pushvalue(state, 1);
	lua_pushinteger(state, 2);
	lua_call(state, 2, 1);

	return 1;
}

bool pemsa_optional_bool(lua_State* state, int index, bool defaultValue) {
	if (lua_isboolean(state, index)) {
		return lua_toboolean(state, index);
	}

	return defaultValue;
}