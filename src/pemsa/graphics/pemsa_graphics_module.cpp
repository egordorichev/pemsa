#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <mutex>
#include <iostream>

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

void PemsaGraphicsModule::displayError(const char* error) {
	emulator->getDrawStateModule()->reset();

	lua_State* state = this->emulator->getCartridgeModule()->getCart()->state;
	lua_getglobal(state, "__error");

	if (lua_isnil(state, -1)) {
		lua_pop(state, 1);
	} else {
		lua_pushstring(state, error);

		if (lua_pcall(state, 1, 0, lua_gettop(state) - 2) != 0) {
			std::cerr << "Error function error: " << lua_tostring(state, -1) << "\n";
		}
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