#include "pemsa/pemsa_emulator.hpp"
#include <cmath>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif

static PemsaEmulator* emulator;

static int rnd(lua_State* state) {
	pemsa_pushnumber(state, (double) rand() / RAND_MAX * pemsa_optnumber(state, 1, 1));
	return 1;
}

static int srand(lua_State* state) {
	srand(pemsa_optnumber(state, 1, 1));
	return 0;
}

static int abs(lua_State* state) {
	double v = pemsa_checknumber(state, 1);
	pemsa_pushnumber(state, v < 0 ? v * -1 : v);
	return 1;
}

static int flr(lua_State* state) {
	pemsa_pushnumber(state, floor(pemsa_optnumber(state, 1, 0)));
	return 1;
}

static int ceil(lua_State* state) {
	pemsa_pushnumber(state, ceil(pemsa_optnumber(state, 1, 0)));
	return 1;
}

static int sgn(lua_State* state) {
	pemsa_pushnumber(state, pemsa_optnumber(state, 1, 1) < 0 ? -1 : 1);
	return 1;
}

static int atan2(lua_State* state) {
	double angle = atan2(pemsa_checknumber(state, 1), pemsa_checknumber(state, 2));
	pemsa_pushnumber(state, fmod((angle) / (M_PI * 2) - 0.25, 2.0));

	return 1;
}

static fix16_t check_number_or_bool(lua_State* state, int n) {
	if (lua_isboolean(state, n)) {
		return fix16_from_int(lua_toboolean(state, n));
	}

	return pemsa_checknumbe_raw(state, n);
}

static int band(lua_State* state) {
	fix16_t a = check_number_or_bool(state, 1);
	fix16_t b = check_number_or_bool(state, 2);

	printf("%f & %f = %f\n", fix16_to_float(a), fix16_to_float(b), fix16_to_float(fix16_band(a, b)));

	pemsa_pushnumber_raw(state, fix16_band(a, b));
	return 1;
}

static int bnot(lua_State* state) {
	pemsa_pushnumber_raw(state, fix16_bnot(check_number_or_bool(state, 1)));
	return 1;
}

static int bor(lua_State* state) {
	pemsa_pushnumber_raw(state, fix16_bor(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
	return 1;
}

static int bxor(lua_State* state) {
	pemsa_pushnumber_raw(state, fix16_bxor(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
	return 1;
}

static int shl(lua_State* state) {
	printf("%f >> %f = %f\n", fix16_to_float(check_number_or_bool(state, 1)), fix16_to_float(check_number_or_bool(state, 2)), fix16_to_float(fix16_shl(check_number_or_bool(state, 1), check_number_or_bool(state, 2))));

	pemsa_pushnumber_raw(state, fix16_shl(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
	return 1;
}

static int shr(lua_State* state) {
	pemsa_pushnumber_raw(state, fix16_shr(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
	return 1;
}

static int cos(lua_State* state) {
	pemsa_pushnumber(state, cos(pemsa_checknumber(state, 1) * M_PI * 2));
	return 1;
}

static int sin(lua_State* state) {
	pemsa_pushnumber(state, -sin(pemsa_checknumber(state, 1) * M_PI * 2));
	return 1;
}

static int sqrt(lua_State* state) {
	pemsa_pushnumber(state, sqrt(pemsa_checknumber(state, 1)));
	return 1;
}

static int min(lua_State* state) {
	pemsa_pushnumber(state, fmin(pemsa_checknumber(state, 1), pemsa_checknumber(state, 2)));
	return 1;
}

static int max(lua_State* state) {
	pemsa_pushnumber(state, fmax(pemsa_checknumber(state, 1), pemsa_checknumber(state, 2)));
	return 1;
}

static int mid(lua_State* state) {
	double x = pemsa_checknumber(state, 1);
	double y = pemsa_checknumber(state, 2);
	double z = pemsa_checknumber(state, 3);

	if (x > y) {
		double tmp = x;
		x = y;
		y = tmp;
	}

	pemsa_pushnumber(state, fmax(x, fmin(y, z)));
	return 1;
}

void pemsa_open_math_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "rnd", rnd);
	lua_register(state, "srand", srand);
	lua_register(state, "abs", abs);
	lua_register(state, "flr", flr);
	lua_register(state, "ceil", ceil);
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