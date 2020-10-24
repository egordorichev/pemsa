#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <mutex>

PemsaGraphicsModule::PemsaGraphicsModule(PemsaEmulator* emulator, PemsaGraphicsBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
	this->backend->createSurface();
}

PemsaGraphicsModule::~PemsaGraphicsModule() {
	delete this->backend;
}

void PemsaGraphicsModule::update(double dt) {
	PemsaCartridgeModule* cartridgeModule = this->emulator->getCartridgeModule();
	std::unique_lock<std::mutex> uniqueLock(*cartridgeModule->getMutex());

	this->backend->flip();
	cartridgeModule->getLock()->notify_one();
}
