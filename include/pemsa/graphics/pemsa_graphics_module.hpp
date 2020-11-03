#ifndef PEMSA_GRAPHICS_MODULE_HPP
#define PEMSA_GRAPHICS_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "pemsa/pemsa_module.hpp"
#include "pemsa/graphics/pemsa_graphics_backend.hpp"

enum PemsaDrawMode {
	SCREEN_NORMAL,
	SCREEN_HORIZONTAL_STRETCH,
	SCREEN_VERTICAL_STRETCH,
	SCREEN_STRETCH,
	SCREEN_UNUSED,
	SCREEN_HORIZONTAL_MIRROR,
	SCREEN_VERTICAL_MIRROR,
	SCREEN_MIRROR,

	SCREEN_HORIZONTAL_FLIP,
	SCREEN_VERTICAL_FLIP,
	SCREEN_90_ROTATION,
	SCREEN_180_ROTATION,
	SCREEN_270_ROTATION
};

class PemsaGraphicsModule : public PemsaModule {
	public:
		PemsaGraphicsModule(PemsaEmulator* emulator, PemsaGraphicsBackend* backend);
		~PemsaGraphicsModule();

		void update(double dt) override;
		PemsaGraphicsBackend* getBackend();

		PemsaDrawMode getDrawMode();
	private:
		PemsaGraphicsBackend* backend;
};

void pemsa_open_graphics_api(PemsaEmulator* machine, lua_State* state);

#endif