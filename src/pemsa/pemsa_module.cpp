#include "pemsa/pemsa_module.hpp"

PemsaModule::PemsaModule(PemsaEmulator *emulator) {
	this->emulator = emulator;
}

void PemsaModule::defineApi(lua_State* state) {

}

void PemsaModule::init() {

}

void PemsaModule::update(double dt) {

}

void PemsaModule::onCartridgeLoaded() {

}

void PemsaModule::reset() {

}