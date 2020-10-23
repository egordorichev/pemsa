#include "pemsa/cart/pemsa_cartridge_module.hpp"

PemsaCartridgeModule::PemsaCartridgeModule(PemsaEmulator *emulator) : PemsaModule(emulator) {

}

bool PemsaCartridgeModule::load(const char *path) {
	return true;
}