#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <mutex>

#define PEMSA_LOW_FPS_DELTA 1 / 30.0
#define PEMSA_HIGH_FPS_DELTA 1 / 60.0

PemsaGraphicsModule::PemsaGraphicsModule(PemsaEmulator* emulator, PemsaGraphicsBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
	this->backend->createSurface();
	this->time = 0;
}

PemsaGraphicsModule::~PemsaGraphicsModule() {
	delete this->backend;
}

void PemsaGraphicsModule::update(double dt) {
	this->time += dt;

	PemsaCartridgeModule* cartridgeModule = this->emulator->getCartridgeModule();

	double renderDelta = (cartridgeModule->getCart() != nullptr && cartridgeModule->getCart()->highFps) ? PEMSA_HIGH_FPS_DELTA : PEMSA_LOW_FPS_DELTA;

	if (this->time >= renderDelta) {
		this->time -= renderDelta;
		this->emulator->getCartridgeModule()->allowExecutionOfNextFrame();
	}
}

PemsaGraphicsBackend *PemsaGraphicsModule::getBackend() {
	return this->backend;
}

PemsaDrawMode PemsaGraphicsModule::getDrawMode() {
	int mode = this->emulator->getMemoryModule()->ram[PEMSA_RAM_DRAW_MODE];

	if (mode > 128) {
		return (PemsaDrawMode) (mode - 128 + SCREEN_MIRROR);
	}

	return (PemsaDrawMode) mode;
}