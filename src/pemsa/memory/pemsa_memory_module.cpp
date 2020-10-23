#include "pemsa/memory/pemsa_memory_module.hpp"
#include <cstring>

PemsaMemoryModule::PemsaMemoryModule(PemsaEmulator *emulator) : PemsaModule(emulator) {
	memset(this->ram, 0, PEMSA_RAM_END);
}