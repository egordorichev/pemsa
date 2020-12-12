#include "pemsa/cart/pemsa_scanner.hpp"
#include "pemsa/cart/pemsa_emitter.hpp"
#include "pemsa/cart/pemsa_cartridge_module.hpp"
#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/pemsa_emulator.hpp"
#include "pemsa/util/pemsa_font.hpp"

#include "lualib.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
#include <sstream>
#endif

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

inline std::string& ltrim(std::string& s) {
	s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
	return s;
}

static void print_line(uint8_t* buffer, const char* text, int x, int y, int c) {
	int offsetX = 0;
	int offsetY = 0;

	while (*text != '\0' && *text != '\n') {
		uint8_t cr = *((uint8_t*) text);
		text++;

		if (cr >= 'A' && cr <= 'z') {
			if (cr >= 'a') {
				cr = toupper(cr);
			} else {
				cr = tolower(cr);
			}
		}

		const char** letter = pemsa_get_letter(cr);

		if (letter != nullptr) {
			int w = cr > 127 ? 7 : 3;

			for (int ly = 0; ly < 5; ly++) {
				for (int lx = 0; lx < w; lx++) {
					if (letter[ly][lx] == 'x') {
						int xx = x + lx + offsetX;
						int yy = y + ly + offsetY;

						int index = xx / 2 + yy * (PEMSA_CREDITS_WIDTH / 2);

						if (index >= PEMSA_CREDITS_HALF_SIZE || xx >= PEMSA_CREDITS_WIDTH || yy >= PEMSA_CREDITS_HEIGHT) {
							return;
						}

						buffer[index] = SET_HALF(buffer[index], c, (xx & 1) == 0);
					}
				}
			}

			offsetX += w + 1;
		} else {
			offsetX += 4;
		}
	}
}

PemsaCartridgeModule::PemsaCartridgeModule(PemsaEmulator *emulator) : PemsaModule(emulator) {
	this->destruct = false;
}

PemsaCartridgeModule::~PemsaCartridgeModule() {
	this->cleanupCart();
}

void PemsaCartridgeModule::update(double dt) {
	if (this->cart != nullptr) {
		this->cart->time += dt;
	}

	if (this->destruct) {
		this->destruct = false;
		cleanupCart();

		this->emulator->reset();
		this->paused = false;

		load(lastLoaded);
	}
}

bool PemsaCartridgeModule::load(const char *path, bool onlyLoad) {
	std::ifstream file(path);

	if (file.bad() || !file.is_open()) {
		file.close();
		file.open(path + std::string(".p8"));

		if (file.bad() || !file.is_open()) {
			std::cerr << "Failed to open the file\n";
			return false;
		}
	}

	std::string line;

	if (!std::getline(file, line) || memcmp(line.c_str(), "pico-8 cartridge", 16) != 0) {
		std::cerr << "Invalid file header\n";
		return false;
	}

	this->cart = new PemsaCartridge();
	this->cart->time = 0;
	this->cart->name = nullptr;
	this->cart->author = nullptr;

	if (!std::getline(file, line) || memcmp(line.c_str(), "version", 7) != 0) {
		// TODO: read version from the line and put it in cart rom
		std::cerr << "Invalid file header\n";
		return false;
	}

	std::stringstream code;
	std::stringstream label;

	int cartState = -1;
	int index = 0;
	bool codePreformatted = false;
	int codeLineNumber = 1;

	uint8_t* rom = this->cart->rom;

	while (std::getline(file, line)) {
		const char* cline = line.c_str();
		size_t length = line.size();

		// Smallest label is __lua__ (7 chars)
		if (length > 6 && memcmp(cline, "__", 2) == 0) {
			int old_state = cartState;
			// Skip the __?
			cline += 3;

			switch (cline[-1]) {
				case 'l': {
					if (memcmp(cline, "ua__", 4) == 0) {
						cartState = STATE_LUA;
					} else if (memcmp(cline, "abel__", 6) == 0) {
						cartState = STATE_LABEL;
					}

					break;
				}

				case 'g': {
					if (memcmp(cline, "fx__", 4) == 0) {
						cartState = STATE_GFX;
					} else if (memcmp(cline, "ff__", 4) == 0) {
						cartState = STATE_GFF;
					}

					break;
				}

				case 'm': {
					if (memcmp(cline, "ap__", 4) == 0) {
						cartState = STATE_MAP;
					} else if (memcmp(cline, "usic__", 6) == 0) {
						cartState = STATE_MUSIC;
					}

					break;
				}

				case 's': {
					if (memcmp(cline, "fx__", 4) == 0) {
						cartState = STATE_SFX;
					}

					break;
				}

				case 'c': {
					if (memcmp(cline, "ode__", 5) == 0) {
						cartState = STATE_LUA;
						codePreformatted = true;
					}

					break;
				}
			}

			if (cartState != old_state) {

				index = 0;
				continue;
			}
		}

		switch (cartState) {
			case STATE_LUA: {
				if (codeLineNumber < 3) {
					std::string trimmedLine = ltrim(line);

					if (trimmedLine.size() > 2 && (trimmedLine.rfind("--", 0) || trimmedLine.rfind("//", 0))) {
						trimmedLine = trimmedLine.substr(2);
						trimmedLine = ltrim(trimmedLine);

						if (cart->name == nullptr) {
							cart->name = take_string(trimmedLine);
						} else {
							cart->author = take_string(trimmedLine);
						}
					}
				}

				code << line << "\n";
				codeLineNumber++;

				break;
			}

			case STATE_LABEL: {
				label << line << "\n";
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
					rom[(index >= 0x1000 ? PEMSA_ROM_GFX : PEMSA_ROM_MAP) + index] = (HEX_TO_INT(line.at(i)) << 4) + HEX_TO_INT(line.at(i + 1));
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

			case STATE_SFX: {
				if (line.size() < 8) {
					break;
				}

				uint8_t editor = (HEX_TO_INT(line.at(0)) << 4) + HEX_TO_INT(line.at(1));
				uint8_t speed = (HEX_TO_INT(line.at(2)) << 4) + HEX_TO_INT(line.at(3));
				uint8_t startLoop = (HEX_TO_INT(line.at(4)) << 4) + HEX_TO_INT(line.at(5));
				uint8_t endLoop = (HEX_TO_INT(line.at(6)) << 4) + HEX_TO_INT(line.at(7));

				rom[PEMSA_ROM_SFX + index * 68 + 64] = editor;
				rom[PEMSA_ROM_SFX + index * 68 + 65] = speed;
				rom[PEMSA_ROM_SFX + index * 68 + 66] = startLoop;
				rom[PEMSA_ROM_SFX + index * 68 + 67] = endLoop;

				int off = 0;

				for (int i = 0; i < line.size() - 8; i += 5) {
					uint8_t pitch = (HEX_TO_INT(line.at(i + 8)) << 4) + HEX_TO_INT(line.at(i + 9));
					uint8_t waveform = HEX_TO_INT(line.at(i + 10));
					uint8_t volume = HEX_TO_INT(line.at(i + 11));
					uint8_t effect = HEX_TO_INT(line.at(i + 12));

					bool custom = waveform > 7;
					waveform = waveform % 8;

					uint8_t lo = (pitch | (waveform << 6));
					uint8_t hi = ((waveform >> 2) | (volume << 1) | (effect << 4));

					if (custom) {
						SET_BIT(hi, 7, true)
					}

					rom[PEMSA_ROM_SFX + index * 68 + off] = lo;
					rom[PEMSA_ROM_SFX + index * 68 + off + 1] = hi;

					off += 2;
				}

				index++;
				break;
			}

			case STATE_MUSIC: {
				if (line.size() < 11) {
					break;
				}

				uint8_t flag = (HEX_TO_INT(line.at(0)) << 4) + HEX_TO_INT(line.at(1));
				uint8_t val1 = (HEX_TO_INT(line.at(3)) << 4) + HEX_TO_INT(line.at(4));
				uint8_t val2 = (HEX_TO_INT(line.at(5)) << 4) + HEX_TO_INT(line.at(6));
				uint8_t val3 = (HEX_TO_INT(line.at(7)) << 4) + HEX_TO_INT(line.at(8));
				uint8_t val4 = (HEX_TO_INT(line.at(9)) << 4) + HEX_TO_INT(line.at(10));

				// 4th byte never has 7th bit set because it's corresponding flag value is never used.
				if ((flag & 0x1) != 0) {
					val1 |= 0x80;
				}

				if ((flag & 0x2) != 0) {
					val2 |= 0x80;
				}

				if ((flag & 0x4) != 0) {
					val3 |= 0x80;
				}

				rom[PEMSA_ROM_SONG + index + 0] = val1;
				rom[PEMSA_ROM_SONG + index + 1] = val2;
				rom[PEMSA_ROM_SONG + index + 2] = val3;
				rom[PEMSA_ROM_SONG + index + 3] = val4;

				index += 4;
				break;
			}

			default: {
				break;
			}
		}
	}

	this->paused = false;
	this->lastLoaded = path;

	lua_State* state = luaL_newstate();

	this->cart->state = state;
	this->cart->cartDataId = take_string(std::filesystem::path(path).stem().string());
	this->cart->fullPath = path;
	this->cart->label = take_string(label.str());

	std::string codeString = code.str();

	if (!codePreformatted) {
		PemsaScanner scanner(codeString.c_str());
		codeString = pemsa_emit(&scanner);
	}

#ifdef PEMSA_SAVE_CODE
	std::ofstream codeFile("code.lua", std::ios::trunc);
	codeFile << codeString;
	codeFile.close();
#endif

	this->cart->code = take_string(codeString);
	this->cart->codeLength = codeString.length();

	if (onlyLoad) {
		return true;
	}

	// TODO: tbh should limit the functions from here?
	luaL_openlibs(state);

	pemsa_open_graphics_api(emulator, state);
	pemsa_open_system_api(emulator, state);
	pemsa_open_math_api(emulator, state);
	pemsa_open_input_api(emulator, state);
	pemsa_open_memory_api(emulator, state);
	pemsa_open_draw_state_api(emulator, state);
	pemsa_open_cartridge_api(emulator, state);
	pemsa_open_audio_api(emulator, state);

	memcpy(emulator->getMemoryModule()->ram, rom, 0x4300);

	this->gameThread = new std::thread(&PemsaCartridgeModule::gameLoop, this);

	return true;
}

std::mutex *PemsaCartridgeModule::getMutex() {
	return &this->mutex;
}

PemsaCartridge *PemsaCartridgeModule::getCart() {
	return this->cart;
}

void PemsaCartridgeModule::gameLoop() {
	this->threadRunning = true;
	this->cart->time = 1 / 30.0;

	lua_State* state = this->cart->state;

	if (luaL_loadbuffer(state, this->cart->code, this->cart->codeLength, "=cart") != 0) {
		this->reportLuaError();
		return;
	}

	if (lua_pcall(state, 0, 0, lua_gettop(state) - 1) != 0) {
		this->reportLuaError();
		return;
	}

	this->cart->highFps = this->globalExists("_update60");

	if (this->cart->highFps) {
		this->cart->time = 1 / 60.0;
	}

	// We need to update the button press states for some edgecases like goto loops before
	// the actual cart, like in bingle jells. without this the menu will be skipped
	this->emulator->getInputModule()->updateInput();
	this->callIfExists("_init");

	while (this->threadRunning) {
		if (!this->paused) {
			if (this->cart->highFps) {
				this->callIfExists("_update60");
			} else {
				this->callIfExists("_update");
			}

			this->callIfExists("_draw");
		}

		this->flip();
	}
}

void PemsaCartridgeModule::cleanupCart() {
	if (this->cart == nullptr) {
		return;
	}

	this->threadRunning = false;
	this->lock.notify_all();
	this->gameThread->join();

	lua_close(this->cart->state);

	delete this->gameThread;
	delete this->cart->code;
	delete this->cart->cartDataId;
	delete this->cart->name;
	delete this->cart->author;
	delete this->cart;
}

void PemsaCartridgeModule::callIfExists(const char *method_name) {
	lua_State* state = this->cart->state;
	lua_getglobal(state, method_name);

	if (lua_isnil(state, -1)) {
		lua_pop(state, 1);
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

void PemsaCartridgeModule::loadData(const char *path) {
	if (this->cart == nullptr) {
		return;
	}

	this->cart->cartDataId = path;
	std::string fullPath = std::string(PEMSA_CART_DATA_PATH) + "/" + std::string(path);
	std::ifstream file(fullPath);

	fix16_t* data = this->cart->cartData;

	if (file.good() && file.is_open()) {
		for (int i = 0; i < PEMSA_CART_DATA_SIZE; i++) {
			file >> data[i];
		}

		file.close();
	} else {
		for (int i = 0; i < PEMSA_CART_DATA_SIZE; i++) {
			data[i] = 0;
		}

		this->saveData();
	}
}

void PemsaCartridgeModule::saveData() {
	if (this->cart == nullptr) {
		return;
	}

	std::filesystem::create_directory(PEMSA_CART_DATA_PATH);
	std::string fullPath = std::string(PEMSA_CART_DATA_PATH) + "/" + std::string(this->cart->cartDataId);
	std::ofstream file(fullPath);

	if (file.bad() || !file.is_open()) {
		std::cerr << "Failed to save cart data\n";
		return;
	}

	fix16_t* data = this->cart->cartData;

	for (int i = 0; i < PEMSA_CART_DATA_SIZE; i++) {
		file << data[i] << "\n";
	}

	file.close();
}

void PemsaCartridgeModule::stop() {
	this->threadRunning = false;
}

void PemsaCartridgeModule::flip() {
	this->callIfExists("__update_menu");

	this->waiting = true;
	std::unique_lock<std::mutex> uniqueLock(this->mutex);
	this->lock.wait(uniqueLock, [this] { return !this->waiting || !this->threadRunning; });
}

void PemsaCartridgeModule::setPaused(bool paused) {
	this->paused = paused;
}

void PemsaCartridgeModule::initiateSelfDestruct() {
	this->destruct = true;
}

void PemsaCartridgeModule::notify() {
	this->waiting = false;
	this->lock.notify_all();
}

bool PemsaCartridgeModule::save(const char* path, bool useCodeTag) {
	if (this->cart == nullptr) {
		return false;
	}

	std::ofstream file(path, std::ios::binary);

	if (file.bad() || !file.is_open()) {
		std::cerr << "Failed to save the cart to " << path << "\n";
		return false;
	}

	file << "pico-8 cartridge // http://www.pico-8.com\n";
	file << "version 29\n";

	file << (useCodeTag ? "__code__" : "__lua__");
	file << this->cart->code;

	file << "\n__gfx__\n";

	uint8_t* rom = this->cart->rom;

	for (int j = 0; j < 128; j++) {
		for (int i = 0; i < 64; i++) {
			uint8_t byte = rom[j * 64 + i + PEMSA_ROM_GFX];
			file << INT_TO_HEX(GET_HALF(byte, true)) << INT_TO_HEX(GET_HALF(byte, false));
		}

		file << "\n";
	}

	file << "__gff__\n";

	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 128; i++) {
			uint8_t byte = rom[j * 128 + i + PEMSA_ROM_GFX_PROPS];
			file << INT_TO_HEX(GET_HALF(byte, false)) << INT_TO_HEX(GET_HALF(byte, true));
		}

		file << "\n";
	}

	file << "__label__\n" << cart->label << "__credits__\n";

	uint8_t creditsBuffer[PEMSA_CREDITS_HALF_SIZE];
	memset(creditsBuffer, 5 + (5 << 4), PEMSA_CREDITS_HALF_SIZE);

	print_line(creditsBuffer, cart->name, 3, 4, 7);
	print_line(creditsBuffer, cart->author, 3, 12, 7);
	print_line(creditsBuffer, "pico-8 cartidge", 3, 24, 6);

	for (int j = 0; j < PEMSA_CREDITS_HEIGHT; j++) {
		for (int i = 0; i < PEMSA_CREDITS_WIDTH / 2; i++) {
			uint8_t byte = creditsBuffer[j * (PEMSA_CREDITS_WIDTH / 2) + i];
			file << INT_TO_HEX(GET_HALF(byte, true)) << INT_TO_HEX(GET_HALF(byte, false));
		}

		file << "\n";
	}

	file << "__map__\n";

	for (int j = 0; j < 64; j++) {
		for (int i = 0; i < 64; i++) {
			uint8_t byte = rom[j * 64 + i + PEMSA_ROM_MAP];
			file << INT_TO_HEX(GET_HALF(byte, false)) << INT_TO_HEX(GET_HALF(byte, true));
		}

		if (j % 2 == 1) {
			file << "\n";
		}
	}

	file << "__sfx__\n";

	for (int j = 0; j < 64; j++) {
		uint8_t editor = rom[PEMSA_ROM_SFX + j * 68 + 64];
		uint8_t speed = rom[PEMSA_ROM_SFX + j * 68 + 65];
		uint8_t startLoop = rom[PEMSA_ROM_SFX + j * 68 + 66];
		uint8_t endLoop = rom[PEMSA_ROM_SFX + j * 68 + 67];

		file << INT_TO_HEX(GET_HALF(editor, false)) << INT_TO_HEX(GET_HALF(editor, true));
		file << INT_TO_HEX(GET_HALF(speed, false)) << INT_TO_HEX(GET_HALF(speed, true));
		file << INT_TO_HEX(GET_HALF(startLoop, false)) << INT_TO_HEX(GET_HALF(startLoop, true));
		file << INT_TO_HEX(GET_HALF(endLoop, false)) << INT_TO_HEX(GET_HALF(endLoop, true));

		for (int i = 0; i < 64; i += 2) {
			uint8_t lo = rom[PEMSA_ROM_SFX + j * 68 + i];
			uint8_t high = rom[PEMSA_ROM_SFX + j * 68 + i + 1];

			uint8_t pitch = (uint8_t) (lo & 0b00111111);
			uint8_t waveform = (uint8_t) (((lo & 0b11000000) >> 6) | ((high & 0b1) << 2));
			uint8_t volume = (uint8_t) ((high & 0b00001110) >> 1);
			uint8_t effect = (uint8_t) ((high & 0b01110000) >> 4);

			file << INT_TO_HEX(GET_HALF(pitch, false)) << INT_TO_HEX(GET_HALF(pitch, true));
			file << INT_TO_HEX(waveform) << INT_TO_HEX(volume) << INT_TO_HEX(effect);
		}

		file << "\n";
	}

	file << "__music__\n";

	for (int j = 0; j < 64; j++) {
		uint8_t flag = 0;
		uint8_t val0 = rom[j * 4 + 0 + PEMSA_ROM_SONG];
		uint8_t val1 = rom[j * 4 + 1 + PEMSA_ROM_SONG];
		uint8_t val2 = rom[j * 4 + 2 + PEMSA_ROM_SONG];
		uint8_t val3 = rom[j * 4 + 3 + PEMSA_ROM_SONG];

		if ((val0 & 0x80) == 0x80) {
			flag |= 1;
			val0 &= 0x7F;
		}

		if ((val1 & 0x80) == 0x80) {
			flag |= 2;
			val1 &= 0x7F;
		}

		if ((val2 & 0x80) == 0x80) {
			flag |= 4;
			val2 &= 0x7F;
		}


		char s[3];
		sprintf(s, "%02i", flag);

		file << s << " ";

		file << INT_TO_HEX(GET_HALF(val0, false)) << INT_TO_HEX(GET_HALF(val0, true));
		file << INT_TO_HEX(GET_HALF(val1, false)) << INT_TO_HEX(GET_HALF(val1, true));
		file << INT_TO_HEX(GET_HALF(val2, false)) << INT_TO_HEX(GET_HALF(val2, true));
		file << INT_TO_HEX(GET_HALF(val3, false)) << INT_TO_HEX(GET_HALF(val3, true)) << "\n";
	}

	file.close();
	return true;
}