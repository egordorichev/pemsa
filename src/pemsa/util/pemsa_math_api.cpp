#include "pemsa/pemsa_emulator.hpp"
#include <cmath>

static PemsaEmulator* emulator;

/*
 * todo:
 * cos(x)
 * sin(x)
 * flr(x)
 * max(x, y)
 * mid(x, y, z)
 * min(x, y)
 * sgn(x)
 * shl(x, y)
 * shr(x, y)
 * sqrt(x)
 */

static int rnd(lua_State* state) {
	lua_pushnumber(state, fmod(rand(), luaL_optnumber(state, 1, 1)));
	return 1;
}

static int srand(lua_State* state) {
	srand(luaL_optnumber(state, 1, 1));
	return 0;
}

static int abs(lua_State* state) {
	lua_pushnumber(state, abs(luaL_checknumber(state, 1)));
	return 1;
}

static int atan2(lua_State* state) {
	double angle = atan2(luaL_checknumber(state, 2), luaL_checknumber(state, 1));
	lua_pushnumber(state, fmod((0.75 + angle) / (M_PI * 2), 1.0));

	return 1;
}

static int band(lua_State* state) {
	lua_pushnumber(state, (int) luaL_checknumber(state, 1) & (int) luaL_checknumber(state, 2));
	return 1;
}

static int bnot(lua_State* state) {
	lua_pushnumber(state, ~((int) luaL_checknumber(state, 1)));
	return 1;
}

static int bor(lua_State* state) {
	lua_pushnumber(state, (int) luaL_checknumber(state, 1) | (int) luaL_checknumber(state, 2));
	return 1;
}

static int bxor(lua_State* state) {
	lua_pushnumber(state, (int) luaL_checknumber(state, 1) ^ (int) luaL_checknumber(state, 2));
	return 1;
}

static int cos(lua_State* state) {
	lua_pushnumber(state, cos(luaL_checknumber(state, 1) * M_PI * 2));
	return 1;
}

static int sin(lua_State* state) {
	lua_pushnumber(state, -sin(luaL_checknumber(state, 1) * M_PI * 2));
	return 1;
}

void pemsa_open_math_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "rnd", rnd);
	lua_register(state, "srand", srand);
	lua_register(state, "abs", abs);
	lua_register(state, "atan2", atan2);
	lua_register(state, "band", band);
	lua_register(state, "bnot", bnot);
	lua_register(state, "bor", bor);
	lua_register(state, "bxor", bxor);
	lua_register(state, "cos", cos);
	lua_register(state, "sin", sin);
}