#include "pemsa/pemsa_emulator.hpp"

#include <cmath>
#include <string.h>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif

static PemsaEmulator* emulator;

static int rnd(lua_State* state) {
	pemsa_pushnumber(state, (float) rand() / RAND_MAX * pemsa_optnumber(state, 1, 1));
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
	if (lua_gettop(state) >= n) {
		if (lua_isboolean(state, n)) {
			return fix16_from_int(lua_toboolean(state, n));
		}

		if (lua_isnil(state, n)) {
			return fix16_from_int(0);
		}

		return pemsa_checknumber_raw(state, n);
	}

	return n;
}

static int band(lua_State* state) {
	fix16_t a = check_number_or_bool(state, 1);
	fix16_t b = check_number_or_bool(state, 2);

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
	pemsa_pushnumber_raw(state, fix16_shl(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
	return 1;
}

static int shr(lua_State* state) {
	pemsa_pushnumber_raw(state, fix16_shr(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
	return 1;
}

static int rotl(lua_State* state) {
	pemsa_pushnumber_raw(state, fix16_rotl(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
	return 1;
}

static int rotr(lua_State* state) {
	pemsa_pushnumber_raw(state, fix16_rotr(check_number_or_bool(state, 1), check_number_or_bool(state, 2)));
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
	pemsa_pushnumber_raw(state, fix16_sqrt(pemsa_checknumber_raw(state, 1)));
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

static int ord(lua_State* state) {
	if (lua_gettop(state) == 0) {
		return 0;
	}

	if (lua_isstring(state, 1) || lua_isnumber(state, 1)) {
		int index = pemsa_optnumber(state, 2, 1) - 1;
		const char* str = lua_tostring(state, 1);

		if (index < 0 || index >= strlen(str)) {
			return 0;
		} else {
			pemsa_pushnumber(state, (int) *(str + index));
			return 1;
		}
	}

	return 0;
}

static int chr(lua_State* state) {
	char str = (char) pemsa_checknumber(state, 1);
	lua_pushstring(state, &str);

	return 1;
}

void pemsa_open_math_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;
	srand(time(nullptr));

	lua_register(state, "__rnd", rnd);
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
	lua_register(state, "lshr", shl);
	lua_register(state, "rotl", rotl);
	lua_register(state, "rotr", rotr);

	lua_register(state, "cos", cos);
	lua_register(state, "sin", sin);
	lua_register(state, "sqrt", sqrt);
	lua_register(state, "min", min);
	lua_register(state, "max", max);
	lua_register(state, "mid", mid);

	lua_register(state, "ord", ord);
	lua_register(state, "chr", chr);
}