#include "pemsa/input/pemsa_input_module.hpp"

PemsaInputModule::PemsaInputModule(PemsaEmulator* emulator, PemsaInputBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
}

PemsaInputModule::~PemsaInputModule() {
	delete this->backend;
}