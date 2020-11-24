#ifndef PEMSA_EMULATOR_HPP
#define PEMSA_EMULATOR_HPP

#include "pemsa/pemsa_module.hpp"
#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/input/pemsa_input_module.hpp"
#include "pemsa/cart/pemsa_cartridge_module.hpp"
#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/memory/pemsa_draw_state_module.hpp"
#include "pemsa/util/pemsa_util.hpp"
#include "lua.h"

#define PEMSA_MODULE_COUNT 6

class PemsaEmulator {
	public:
		PemsaEmulator(PemsaGraphicsBackend* graphics, PemsaAudioBackend *audio, PemsaInputBackend *input, bool* running);
		~PemsaEmulator();

		void update(double dt);
		void reset();

		PemsaGraphicsModule* getGraphicsModule();
		PemsaAudioModule* getAudioModule();
		PemsaInputModule* getInputModule();
		PemsaCartridgeModule* getCartridgeModule();
		PemsaMemoryModule* getMemoryModule();
		PemsaDrawStateModule* getDrawStateModule();

		void stop();
	private:
		PemsaModule* modules[PEMSA_MODULE_COUNT];

		PemsaGraphicsModule* graphicsModule;
		PemsaAudioModule* audioModule;
		PemsaInputModule* inputModule;
		PemsaCartridgeModule* cartridgeModule;
		PemsaMemoryModule* memoryModule;
		PemsaDrawStateModule* drawStateModule;

		bool* running;
};

const char* pemsa_to_string(lua_State* state, int n);
void pemsa_open_system_api(PemsaEmulator* machine, lua_State* state);
void pemsa_open_math_api(PemsaEmulator* machine, lua_State* state);

#endif