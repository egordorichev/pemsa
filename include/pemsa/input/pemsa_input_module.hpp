#ifndef PEMSA_INPUT_MODULE_HPP
#define PEMSA_INPUT_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "pemsa/pemsa_module.hpp"
#include "pemsa/input/pemsa_input_backend.hpp"

#define PEMSA_BUTTON_COUNT 6
#define PEMSA_PLAYER_COUNT 8

#define PEMSA_BUTTON_LEFT 0
#define PEMSA_BUTTON_RIGHT 1
#define PEMSA_BUTTON_UP 2
#define PEMSA_BUTTON_DOWN 3
#define PEMSA_BUTTON_A 4
#define PEMSA_BUTTON_B 5

class PemsaInputModule : public PemsaModule {
	public:
		PemsaInputModule(PemsaEmulator* emulator, PemsaInputBackend* backend);
		~PemsaInputModule();

		bool isDown(int button, int player);
	private:
		PemsaInputBackend* backend;
};

#endif