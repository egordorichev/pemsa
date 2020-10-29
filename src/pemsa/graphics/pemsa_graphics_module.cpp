#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <mutex>

#define PEMSA_LOW_FPS_DELTA 1 / 30.0
#define PEMSA_HIGH_FPS_DELTA 1 / 60.0

PemsaGraphicsModule::PemsaGraphicsModule(PemsaEmulator* emulator, PemsaGraphicsBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
	this->backend->createSurface();
}

PemsaGraphicsModule::~PemsaGraphicsModule() {
	delete this->backend;
}

void PemsaGraphicsModule::update(double dt) {
	this->time += dt;

	PemsaCartridgeModule* cartridgeModule = this->emulator->getCartridgeModule();
	PemsaCartridge* cart = cartridgeModule->getCart();

	double renderDelta = (cart != nullptr && cartridgeModule->getCart()->highFps) ? PEMSA_HIGH_FPS_DELTA : PEMSA_LOW_FPS_DELTA;

	while (this->time >= renderDelta) {
		this->time -= renderDelta;

		std::unique_lock<std::mutex> uniqueLock(*cartridgeModule->getMutex());

		this->backend->flip();
		cartridgeModule->getLock()->notify_one();
	}
}

PemsaGraphicsBackend *PemsaGraphicsModule::getBackend() {
	return this->backend;
}