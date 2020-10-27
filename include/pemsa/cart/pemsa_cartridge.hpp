#ifndef PEMSA_CARTRIDGE_HPP
#define PEMSA_CARTRIDGE_HPP

#include "lua5.2/lua.hpp"
#include <cstdint>

#define PEMSA_ROM_GFX 0x0
#define PEMSA_ROM_GFX_MAP 0x1000
#define PEMSA_ROM_MAP 0x2000
#define PEMSA_ROM_GFX_PROPS 0x3000
#define PEMSA_ROM_SONG 0x3100
#define PEMSA_ROM_SFX 0x3200
#define PEMSA_ROM_META 0x8000

#define PEMSA_CART_DATA_PATH "cartdata/"
#define PEMSA_CART_DATA_SIZE 64
#define PEMSA_CART_ROM_SIZE 0x8005

struct PemsaCartridge {
	lua_State* state;

	const char* code;
	int codeLength;

	const char* fullPath;
	const char* cartDataId;

	double time;

	uint8_t rom[PEMSA_CART_ROM_SIZE];
	uint8_t cartData[PEMSA_CART_DATA_SIZE];
};

#endif