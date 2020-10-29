#include "pemsa/pemsa_emulator.hpp"
#include <cmath>

static PemsaEmulator* emulator;

static int rnd(lua_State* state) {
	lua_pushnumber(state, (double) rand() / RAND_MAX * luaL_optnumber(state, 1, 1));
	return 1;
}

static int srand(lua_State* state) {
	srand(luaL_optnumber(state, 1, 1));
	return 0;
}

static int abs(lua_State* state) {
	double v = luaL_checknumber(state, 1);
	lua_pushnumber(state, v < 0 ? v * -1 : v);
	return 1;
}

static int flr(lua_State* state) {
	lua_pushnumber(state, (int) luaL_checknumber(state, 1));
	return 1;
}

static int sgn(lua_State* state) {
	lua_pushnumber(state, luaL_checknumber(state, 1) < 0 ? -1 : 1);
	return 1;
}

static int atan2(lua_State* state) {
	double angle = atan2(luaL_checknumber(state, 1), luaL_checknumber(state, 2));
	lua_pushnumber(state, fmod((angle) / (M_PI * 2) - 0.25, 2.0));

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

static int shl(lua_State* state) {
	lua_pushnumber(state, (int) luaL_checknumber(state, 1) << (int) luaL_checknumber(state, 2));
	return 1;
}

static int shr(lua_State* state) {
	lua_pushnumber(state, (int) luaL_checknumber(state, 1) >> (int) luaL_checknumber(state, 2));
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

static int sqrt(lua_State* state) {
	lua_pushnumber(state, sqrt(luaL_checknumber(state, 1)));
	return 1;
}

static int min(lua_State* state) {
	lua_pushnumber(state, fmin(luaL_checknumber(state, 1), luaL_checknumber(state, 2)));
	return 1;
}

static int max(lua_State* state) {
	lua_pushnumber(state, fmax(luaL_checknumber(state, 1), luaL_checknumber(state, 2)));
	return 1;
}

static int mid(lua_State* state) {
	double x = luaL_checknumber(state, 1);
	double y = luaL_checknumber(state, 2);
	double z = luaL_checknumber(state, 3);

	if (x > y) {
		double tmp = x;
		x = y;
		y = tmp;
	}

	lua_pushnumber(state, fmax(x, fmin(y, z)));
	return 1;
}

void pemsa_open_math_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "rnd", rnd);
	lua_register(state, "srand", srand);
	lua_register(state, "abs", abs);
	lua_register(state, "flr", flr);
	lua_register(state, "sgn", sgn);
	lua_register(state, "atan2", atan2);
	lua_register(state, "band", band);
	lua_register(state, "bnot", bnot);
	lua_register(state, "bor", bor);
	lua_register(state, "bxor", bxor);
	lua_register(state, "shl", shl);
	lua_register(state, "shr", shr);
	lua_register(state, "cos", cos);
	lua_register(state, "sin", sin);
	lua_register(state, "sqrt", sqrt);
	lua_register(state, "min", min);
	lua_register(state, "max", max);
	lua_register(state, "mid", mid);
}