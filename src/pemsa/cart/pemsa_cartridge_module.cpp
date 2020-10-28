#include "pemsa/cart/pemsa_scanner.hpp"
#include "pemsa/cart/pemsa_emitter.hpp"
#include "pemsa/cart/pemsa_cartridge_module.hpp"
#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include "lua5.2/lualib.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>

#define STATE_LUA 0
#define STATE_GFX 1
#define STATE_GFF 2
#define STATE_MAP 3
#define STATE_SFX 4
#define STATE_MUSIC 5
#define STATE_LABEL 6

static const char* take_string(std::string str) {
	int length = str.size() + 1;
	char* cstr = new char[length];
	memcpy(cstr, str.c_str(), length);

	return cstr;
}

PemsaCartridgeModule::PemsaCartridgeModule(PemsaEmulator *emulator) : PemsaModule(emulator) {

}

PemsaCartridgeModule::~PemsaCartridgeModule() {
	this->cleanupCart();
}

void PemsaCartridgeModule::update(double dt) {
	if (this->cart != nullptr) {
		this->cart->time += dt;
	}
}

bool PemsaCartridgeModule::load(const char *path) {
	std::ifstream file(path);

	if (file.bad()) {
		std::cerr << "Failed to open the file\n";
		return false;
	}

	std::string line;

	if (!std::getline(file, line) || memcmp(line.c_str(), "pico-8 cartridge", 16) != 0) {
		std::cerr << "Invalid file header\n";
		return false;
	}

	this->cart = new PemsaCartridge();

	if (!std::getline(file, line) || memcmp(line.c_str(), "version", 7) != 0) {
		// TODO: read version from the line and put it in cart rom
		std::cerr << "Invalid file header\n";
		return false;
	}

	std::stringstream code;
	int cart_state = -1;
	int index = 0;
	uint8_t* rom = this->cart->rom;

	while (std::getline(file, line)) {
		const char* cline = line.c_str();
		size_t length = line.size();

		// Smallest label is __lua__ (7 chars)
		if (length > 6 && memcmp(cline, "__", 2) == 0) {
			int old_state = cart_state;
			// Skip the __?
			cline += 3;

			switch (cline[-1]) {
				case 'l': {
					if (memcmp(cline, "ua__", 4) == 0) {
						cart_state = STATE_LUA;
					} else if (memcmp(cline, "usic__", 6) == 0) {
						cart_state = STATE_MUSIC;
					}

					break;
				}

				case 'g': {
					if (memcmp(cline, "fx__", 4) == 0) {
						cart_state = STATE_GFX;
					} else if (memcmp(cline, "ff__", 4) == 0) {
						cart_state = STATE_GFF;
					}

					break;
				}

				case 'm': {
					if (memcmp(cline, "ap__", 4) == 0) {
						cart_state = STATE_MAP;
					}

					break;
				}

				case 's': {
					if (memcmp(cline, "fx__", 4) == 0) {
						cart_state = STATE_SFX;
					}

					break;
				}
			}

			if (cart_state != old_state) {
				index = 0;
				continue;
			}
		}

		switch (cart_state) {
			case STATE_LUA: {
				code << line << "\n";
				break;
			}

			case STATE_GFX: {
				for (char c : line) {
					rom[PEMSA_ROM_GFX + index / 2] = SET_HALF(rom[PEMSA_ROM_GFX + index / 2], HEX_TO_INT(c), index % 2 == 0);
					index++;
				}

				break;
			}

			case STATE_MAP: {
				for (int i = 0; i < line.length(); i += 2) {
					rom[PEMSA_ROM_MAP + index] = (HEX_TO_INT(line.at(i)) << 4) + HEX_TO_INT(line.at(i + 1));
					index++;
				}

				break;
			}

			case STATE_GFF: {
				for (int i = 0; i < line.length(); i += 2) {
					rom[PEMSA_ROM_GFX_PROPS + index] = (HEX_TO_INT(line.at(i)) << 4) + HEX_TO_INT(line.at(i + 1));
					index++;
				}

				break;
			}

			// todo: music & sfx states

			default: {

			}
		}
	}

	lua_State* state = luaL_newstate();

	this->cart->state = state;
	this->cart->cartDataId = take_string(std::filesystem::path(path).stem());
	this->cart->fullPath = path;

	std::string codeString = code.str();
	PemsaScanner scanner(codeString.c_str());

	codeString = pemsa_emit(&scanner);
	std::cout << codeString << "\n";

	/*
	 * todo: patch the code
	 * if(true) -> if true then end
	 * whitespace token
	 * a \ b -> flr(a / b)
	 */

	this->cart->code = take_string(codeString);
	this->cart->codeLength = codeString.length();

	// TODO: tbh should limit the functions from here?
	luaL_openlibs(state);

	pemsa_open_graphics_api(emulator, state);
	pemsa_open_system_api(emulator, state);
	pemsa_open_math_api(emulator, state);
	pemsa_open_input_api(emulator, state);
	pemsa_open_memory_api(emulator, state);
	pemsa_open_draw_state_api(emulator, state);

	memcpy(emulator->getMemoryModule()->ram, rom, 0x4300);
	this->gameThread = new std::thread(&PemsaCartridgeModule::gameLoop, this);

	return true;
}

std::condition_variable *PemsaCartridgeModule::getLock() {
	return &this->lock;
}

std::mutex *PemsaCartridgeModule::getMutex() {
	return &this->mutex;
}

PemsaCartridge *PemsaCartridgeModule::getCart() {
	return this->cart;
}

void PemsaCartridgeModule::gameLoop() {
	this->threadRunning = true;

	lua_State* state = this->cart->state;
	lua_pushcfunction(state, pemsa_trace_lua);

	if (luaL_loadbuffer(state, this->cart->code, this->cart->codeLength, "=cart") != 0) {
		this->reportLuaError();
		return;
	}

	if (lua_pcall(state, 0, 0, lua_gettop(state) - 1) != 0) {
		this->reportLuaError();
		return;
	}

	bool highFps = this->globalExists("_update60");
	this->callIfExists("_init");

	while (this->threadRunning) {
		if (highFps) {
			this->callIfExists("_update60");
		} else {
			this->callIfExists("_update");
		}

		this->callIfExists("_draw");

		std::unique_lock<std::mutex> uniqueLock(this->mutex);
		this->lock.wait(uniqueLock);
	}
}

void PemsaCartridgeModule::cleanupCart() {
	if (this->cart == nullptr) {
		return;
	}

	lua_close(this->cart->state);

	this->threadRunning = false;
	this->cart = nullptr;

	delete this->gameThread;
	delete this->cart->code;
	delete this->cart->cartDataId;
}

void PemsaCartridgeModule::callIfExists(const char *method_name) {
	lua_State* state = this->cart->state;

	lua_pushcfunction(state, pemsa_trace_lua);
	lua_getglobal(state, method_name);

	if (lua_isnil(state, -1)) {
		lua_pop(state, 2);
	} else {
		if (lua_pcall(state, 0, 0, lua_gettop(state) - 1) != 0) {
			this->reportLuaError();
		}
	}
}

bool PemsaCartridgeModule::globalExists(const char *name) {
	lua_State* state = this->cart->state;

	lua_getglobal(state, name);
	bool exists = !lua_isnil(state, -1);
	lua_pop(state, 1);

	return exists;
}

void PemsaCartridgeModule::reportLuaError() {
	if (this->cart != nullptr) {
		std::cerr << lua_tostring(this->cart->state, -1) << "\n";
		this->threadRunning = false;
	}
}