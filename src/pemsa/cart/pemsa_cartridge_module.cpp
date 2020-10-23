#include "pemsa/cart/pemsa_cartridge_module.hpp"

#include <iostream>

PemsaCartridgeModule::PemsaCartridgeModule(PemsaEmulator *emulator) : PemsaModule(emulator) {

}

PemsaCartridgeModule::~PemsaCartridgeModule() {
	this->cleanupCart();
}

bool PemsaCartridgeModule::load(const char *path) {
	this->cart = new PemsaCartridge();
	this->cart->state = luaL_newstate();

	this->cart->cartDataId = "test";
	this->cart->fullPath = path;
	this->cart->code = "function _init() pset(1, 1, 7) end";

	this->gameThread = new std::thread(&PemsaCartridgeModule::gameLoop, this);

	return true;
}

std::condition_variable *PemsaCartridgeModule::getLock() {
	return &this->lock;
}

std::mutex *PemsaCartridgeModule::getMutex() {
	return &this->mutex;
}

void PemsaCartridgeModule::gameLoop() {
	lua_State* state = this->cart->state;

	if (luaL_dostring(state, this->cart->code) != 0) {
		this->reportLuaError();
		return;
	}

	bool highFps = this->globalExists("_update60");

	while (true) {
		/*if (highFps) {
			this->callIfExists("_update60");
		} else {
			this->callIfExists("_update");
		}

		this->callIfExists("_draw");*/

		std::unique_lock<std::mutex> uniqueLock(this->mutex);
		this->lock.wait(uniqueLock);
	}
}

void PemsaCartridgeModule::cleanupCart() {
	if (this->cart == nullptr) {
		return;
	}

	lua_close(this->cart->state);

	// todo: kill the thread somehow

	delete this->gameThread;
	this->cart = nullptr;
}

void PemsaCartridgeModule::callIfExists(const char *method_name) {
	lua_State* state = this->cart->state;
	lua_getglobal(state, method_name);

	if (lua_isfunction(state, -1)) {
		if (lua_pcall(state, 0, 0, lua_gettop(state) - 1) != 0) {
			this->reportLuaError();
		}
	} else {
		lua_pop(state, 1);
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
	}
}