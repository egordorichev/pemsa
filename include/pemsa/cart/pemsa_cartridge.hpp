#ifndef PEMSA_CARTRIDGE_HPP
#define PEMSA_CARTRIDGE_HPP

#include "lua.hpp"
#include <cstdint>

#define PEMSA_CART_DATA_PATH "cartdata/"
#define PEMSA_CART_DATA_SIZE 64
#define PEMSA_CART_ROM_SIZE 0x8005

struct PemsaCartridge {
	lua_State* state;

	const char* code;
	const char* fullPath;
	const char* cartDataId;

	uint8_t rom[PEMSA_CART_ROM_SIZE];
	uint8_t cartData[PEMSA_CART_DATA_SIZE];
};

#endif