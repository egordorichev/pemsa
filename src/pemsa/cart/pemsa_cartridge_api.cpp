#include "pemsa/pemsa_emulator.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

#ifdef WINDOWS
#include <sstream>
#endif

namespace fs = std::filesystem;

static PemsaEmulator* emulator;

static int dget(lua_State* state) {
	int index = pemsa_checknumber(state, 1);

	if (index >= 0 && index < PEMSA_CART_DATA_SIZE) {
		pemsa_pushnumber_raw(state, emulator->getCartridgeModule()->getCart()->cartData[index]);
	} else {
		pemsa_pushnumber(state, 0);
	}

	return 1;
}

static int dset(lua_State* state) {
	int index = pemsa_checknumber(state, 1);

	if (index >= 0 && index < PEMSA_CART_DATA_SIZE) {
		PemsaCartridgeModule* cartridgeModule = emulator->getCartridgeModule();

		cartridgeModule->getCart()->cartData[index] = pemsa_checknumber_raw(state, 2);
		cartridgeModule->saveData();
	}

	return 0;
}

static int cartdata(lua_State* state) {
	const char* name = luaL_checkstring(state, 1);
	emulator->getCartridgeModule()->loadData(name);

	return 0;
}

static int set_paused(lua_State* state) {
	emulator->getCartridgeModule()->setPaused(pemsa_optional_bool(state, 1, false));
	return 0;
}

static int reset(lua_State* state) {
	emulator->getCartridgeModule()->initiateSelfDestruct();
	return 0;
}

static int load(lua_State* state) {
	const char* name = luaL_checkstring(state, 1);
	emulator->getCartridgeModule()->cleanupAndLoad(name);

	return 0;
}

#ifdef _WIN32
#include <comdef.h>
#endif

static int list_carts(lua_State* state) {
	lua_newtable(state);
	int i = 1;

	for (const auto & entry : fs::directory_iterator("./")) {
		if (entry.is_directory() || entry.path().extension() != ".p8") {
			continue;
		}

		lua_newtable(state);

		auto str = entry.path().stem().c_str();

		#ifdef _WIN32 // Dear windows, just why?
			_bstr_t b(str);
			const char* out = b;
		#else
			const char* out = str;
		#endif

		lua_pushstring(state, out);
		lua_rawseti(state, -2, 1);
		lua_pushstring(state, "egor");
		lua_rawseti(state, -2, 2);

		lua_rawseti(state, -2, i++);
	}

	return 1;
}

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static int read_cdata(lua_State* state) {
	if (lua_gettop(state) == 0) {
		return 0;
	}

	const char* path = pemsa_to_string(state, 1);
	std::ifstream file(path);

	if (file.bad() || !file.is_open()) {
		file.close();
		file.open(path + std::string(".p8"));

		if (file.bad() || !file.is_open()) {
			return 0;
		}
	}

	std::string line;

	for (int i = 0; i < 3; i++) {
		std::getline(file, line); // Skip headers, version & __lua__
	}

	if (line != "__lua__") {
		std::cerr << "No __lua__ mark found as expected\n";
		return 0;
	}

	bool reading_label = false;
	std::stringstream label;

	while (std::getline(file, line)) {
		const char *cline = line.c_str();
		size_t length = line.size();

		// Smallest label is __lua__ (7 chars)
		if (length > 6 && memcmp(cline, "__", 2) == 0) {
			reading_label = (memcmp(cline, "__label__", 9) == 0);
			continue;
		}

		if (reading_label) {
			label << line;
		}
	}

	std::string name;
	std::string author;

	std::getline(file, name);
	std::getline(file, author);

	ltrim(name);
	ltrim(author);
	rtrim(name);
	rtrim(author);

	if (name[0] == '-' && name[1] == '-') {
		name = name.substr(2, name.length());
	} else {
		name = std::string("unknown");
	}

	if (author[0] == '-' && author[1] == '-') {
		author = author.substr(2, author.length());
	} else {
		author = std::string("unknown");
	}

	ltrim(name);
	ltrim(author);
	rtrim(name);
	rtrim(author);

	lua_pushstring(state, name.c_str());
	lua_pushstring(state, author.c_str());
	lua_pushstring(state, label.str().c_str());

	return 3;
}

void pemsa_open_cartridge_api(PemsaEmulator* machine, lua_State* state) {
	emulator = machine;

	lua_register(state, "dget", dget);
	lua_register(state, "dset", dset);
	lua_register(state, "cartdata", cartdata);
	lua_register(state, "__set_paused", set_paused);
	lua_register(state, "__reset", reset);
	lua_register(state, "__load", load);
	lua_register(state, "__list_carts", list_carts);
	lua_register(state, "__read_cdata", read_cdata);
}