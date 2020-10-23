#ifndef PEMSA_EMULATOR_HPP
#define PEMSA_EMULATOR_HPP

#include "pemsa/pemsa_module.hpp"
#include "pemsa/graphics/pemsa_graphics_module.hpp"
#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/input/pemsa_input_module.hpp"
#include "pemsa/cart/pemsa_cartridge_module.hpp"

#define PEMSA_MODULE_COUNT 4

class PemsaEmulator {
	public:
		PemsaEmulator(PemsaGraphicsBackend* graphics, PemsaAudioBackend *audio, PemsaInputBackend *input);
		~PemsaEmulator();

		void update(double dt);

		PemsaGraphicsModule* getGraphicsModule();
		PemsaAudioModule* getAudioModule();
		PemsaInputModule* getInputModule();
		PemsaCartridgeModule* getCartridgeModule();
	private:
		PemsaModule* modules[PEMSA_MODULE_COUNT];

		PemsaGraphicsModule* graphicsModule;
		PemsaAudioModule* audioModule;
		PemsaInputModule* inputModule;
		PemsaCartridgeModule* cartridgeModule;
};

#endif