#include "pemsa/graphics/pemsa_graphics_module.hpp"

PemsaGraphicsModule::PemsaGraphicsModule(PemsaEmulator* emulator, PemsaGraphicsBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
	this->backend->createSurface();
}

PemsaGraphicsModule::~PemsaGraphicsModule() {
	delete this->backend;
}