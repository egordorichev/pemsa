#include "pemsa/memory/pemsa_draw_state_module.hpp"
#include "pemsa/util/pemsa_util.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <cmath>

PemsaEmulator* emulator;

static int pal(lua_State* state) {
	PemsaDrawStateModule* drawStateModule = emulator->getDrawStateModule();

	if (lua_gettop(state) == 0) {
		for (int i = 0; i < 16; i++) {
			drawStateModule->setDrawColor(i, i);
			drawStateModule->setScreenColor(i, i);
			drawStateModule->setTransparent(i, i == 0);
		}

		return 0;
	}

	int color = pemsa_checknumber(state, 1);
	int replacement = pemsa_optnumber(state, 2, 0);
	int p = pemsa_optnumber(state, 3, 0);

	if (p == 0) {
		drawStateModule->setDrawColor(color, replacement);
	} else {
		drawStateModule->setScreenColor(color, replacement);
	}

	return 0;
}

static int palt(lua_State* state) {
	PemsaDrawStateModule* drawStateModule = emulator->getDrawStateModule();

	if (lua_gettop(state) == 0) {
		for (int i = 0; i < 16; i++) {
			drawStateModule->setTransparent(i, i == 0);
		}

		return 0;
	}

	int color = pemsa_checknumber(state, 1);
	bool transparent = pemsa_optional_bool(state, 2, color == 0);

	drawStateModule->setTransparent(color, transparent);
	return 0;
}

static int color(lua_State* state) {
	emulator->getDrawStateModule()->setColor(pemsa_optnumber(state, 1, 6));
	return 0;
}

static int clamp(int min, int max, int v) {
	return fmin(max, fmax(min, v));
}

static int clip(lua_State* state) {
	int x = 0;
	int y = 0;
	int w = 128;
	int h = 128;

	if (lua_gettop(state) != 0) {
		x = pemsa_checknumber(state, 1);
		y = pemsa_checknumber(state, 2);
		w = pemsa_checknumber(state, 3);
		h = pemsa_checknumber(state, 4);
	}

	uint8_t* ram = emulator->getMemoryModule()->ram;

	int nx = clamp(0, 128, x);
	int ny = clamp(0, 128, y);

	ram[PEMSA_RAM_CLIP_LEFT] = nx;
	ram[PEMSA_RAM_CLIP_TOP] = ny;
	ram[PEMSA_RAM_CLIP_RIGHT] = clamp(0, 128, x + w);
	ram[PEMSA_RAM_CLIP_BOTTOM] = clamp(0, 128, y + h);

	return 0;
}

static int fillp(lua_State* state) {
	double p = pemsa_optnumber(state, 1, 0);
	PemsaDrawStateModule* drawStateModule = emulator->getDrawStateModule();

	drawStateModule->setFillPattern(floor(p));
	drawStateModule->setFillPatternTransparent(floor(p) < p);

	return 0;
}

static int camera(lua_State* state) {
	int x = round(pemsa_optnumber(state, 1, 0));
	int y = round(pemsa_optnumber(state, 2, 0));

	PemsaDrawStateModule* drawStateModule = emulator->getDrawStateModule();

	drawStateModule->setCameraX(x);
	drawStateModule->setCameraY(y);

	return 0;
}

static int cursor(lua_State* state) {
	int x = round(pemsa_optnumber(state, 1, 0));
	int y = round(pemsa_optnumber(state, 2, 0));

	PemsaDrawStateModule* drawStateModule = emulator->getDrawStateModule();

	drawStateModule->setCursorX(x);
	drawStateModule->setCursorY(y);

	return 0;
}

void pemsa_open_draw_state_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "palt", palt);
	lua_register(state, "pal", pal);
	lua_register(state, "color", color);
	lua_register(state, "clip", clip);
	lua_register(state, "fillp", fillp);
	lua_register(state, "camera", camera);
	lua_register(state, "cursor", cursor);
}