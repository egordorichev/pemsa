#ifndef PEMSA_CARTRIDGE_HPP
#define PEMSA_CARTRIDGE_HPP

#include "lua.hpp"

#include <string>
#include <cstdint>

#define PEMSA_ROM_GFX 0x0
#define PEMSA_ROM_GFX_MAP 0x1000
#define PEMSA_ROM_MAP 0x2000
#define PEMSA_ROM_GFX_PROPS 0x3000
#define PEMSA_ROM_SONG 0x3100
#define PEMSA_ROM_SFX 0x3200
#define PEMSA_ROM_META 0x8000
#define PEMSA_ROM_END 0x8005

#define PEMSA_CART_DATA_PATH ".cartdata/"
#define PEMSA_CART_DATA_SIZE 64
#define PEMSA_CART_ROM_SIZE PEMSA_ROM_END

#define PEMSA_CREDITS_WIDTH 130
#define PEMSA_CREDITS_HEIGHT 32
#define PEMSA_CREDITS_SIZE (PEMSA_CREDITS_WIDTH * PEMSA_CREDITS_HEIGHT)
#define PEMSA_CREDITS_HALF_SIZE (PEMSA_CREDITS_SIZE / 2)

struct PemsaCartridge {
	lua_State* state;

	const char* code;
	const char* name;
	const char* author;

	int codeLength;

	const char* fullPath;
	const char* cartDataId;

	double time;
	bool highFps;

	uint8_t rom[PEMSA_CART_ROM_SIZE];
	fix16_t cartData[PEMSA_CART_DATA_SIZE];

	const char* label;
};

#endif