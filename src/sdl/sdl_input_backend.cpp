#include "sdl/sdl_input_backend.hpp"
#include "sdl/sdl_graphics_backend.hpp"

#include "pemsa/util/pemsa_util.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <iostream>

// Split the controller DB to bypass ANSI compatibility string character limit.
const char* controller_db =

#if defined(_WIN32)
#include "sdl/sdl_controller_db_windows.hpp"
#elif defined(__linux__)
#include "sdl/sdl_controller_db_linux.hpp"
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
  #if TARGET_OS_MAC
		#include "sdl/sdl_controller_db_mac.hpp"
	#else
		#include "sdl/sdl_controller_db_ios.hpp"
	#endif
#elif defined(__ANDROID__)
#include "sdl/sdl_controller_db_android.hpp"
#endif

SdlInputBackend::SdlInputBackend() {
	SDL_GameControllerAddMappingsFromRW(SDL_RWFromMem((void *) controller_db, strlen(controller_db)), 1);
	this->reset();
}

SdlInputBackend::~SdlInputBackend() {
	for (auto & controller : this->controllers) {
		SDL_GameControllerClose(controller);
	}
}

static int scancode_to_button(SDL_Scancode code, int* player) {
	*player = 0;

	switch (code) {
		case SDL_SCANCODE_LEFT: return 0;
		case SDL_SCANCODE_RIGHT: return 1;
		case SDL_SCANCODE_UP: return 2;
		case SDL_SCANCODE_DOWN: return 3;
		case SDL_SCANCODE_C: case SDL_SCANCODE_Z: return 4;
		case SDL_SCANCODE_X: return 5;
		case SDL_SCANCODE_RETURN: return 6;

		case SDL_SCANCODE_S: *player = 1; return 0;
		case SDL_SCANCODE_F: *player = 1; return 1;
		case SDL_SCANCODE_E: *player = 1; return 2;
		case SDL_SCANCODE_D: *player = 1; return 3;
		case SDL_SCANCODE_LSHIFT: case SDL_SCANCODE_TAB: *player = 1; return 4;
		case SDL_SCANCODE_A: *player = 1; case SDL_SCANCODE_Q: return 5;
	}

	return -1;
}

static int gamepadbutton_to_button(Uint8 button) {
	switch (button) {
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return 0;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return 1;
		case SDL_CONTROLLER_BUTTON_DPAD_UP: return 2;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return 3;
		case SDL_CONTROLLER_BUTTON_B: return 4;
		case SDL_CONTROLLER_BUTTON_A: return 5;
	}

	return -1;
}

void SdlInputBackend::handleEvent(SDL_Event *event) {
	switch (event->type) {
		case SDL_KEYDOWN: {
			this->lastKey = event->key.keysym.scancode;
			this->isDown = true;

			int player;
			int button = scancode_to_button(event->key.keysym.scancode, &player);

			if (button != -1 && this->state[player][button] == 0) {
				this->state[player][button] = 1;
			}

			break;
		}

		case SDL_KEYUP: {
			if (event->key.keysym.scancode == this->lastKey) {
				this->isDown = false;
			}

			int player;
			int button = scancode_to_button(event->key.keysym.scancode, &player);

			if (button != -1) {
				this->state[player][button] = 0;
			}

			break;
		}

		case SDL_CONTROLLERDEVICEADDED: {
			SDL_GameController* controller = SDL_GameControllerOpen(event->cdevice.which);
			std::cout << "Controller (id " << event->cdevice.which << ") " << SDL_GameControllerName(controller) << " added\n";
			this->controllers.push_back(controller);

			break;
		}

		case SDL_CONTROLLERDEVICEREMOVED: {
			std::cout << "Controller " << event->cdevice.which << " removed\n";
			break;
		}

		case SDL_CONTROLLERAXISMOTION: {
			int player = event->caxis.which;

			if (player >= 0 && player < PEMSA_PLAYER_COUNT) {
				float value = event->caxis.value / (float) SDL_MAX_SINT16;
				float absValue = fabs(value);

				if (absValue < 0.25) {
					value = 0;
					absValue = 0;
				}

				switch (event->caxis.axis) {
					case SDL_CONTROLLER_AXIS_LEFTX: {
						if (absValue > 0) {
							this->state[player][value > 0 ? 1 : 0] = 1;
							this->state[player][value > 0 ? 0 : 1] = 0;
						} else {
							this->state[player][1] = 0;
							this->state[player][0] = 0;
						}

						break;
					}

					case SDL_CONTROLLER_AXIS_LEFTY: {
						if (absValue > 0) {
							this->state[player][value > 0 ? 3 : 2] = 1;
							this->state[player][value > 0 ? 2 : 3] = 0;
						} else {
							this->state[player][2] = 0;
							this->state[player][3] = 0;
						}

						break;
					}

					// TODO: mouse emulation with right stick?
				}
			}

			break;
		}

		case SDL_CONTROLLERBUTTONDOWN: {
			int player = event->cbutton.which;

			if (player >= 0 && player < PEMSA_PLAYER_COUNT) {
				int button = gamepadbutton_to_button(event->cbutton.button);

				if (button != -1) {
					this->state[player][button] = 1;
				}
			}

			break;
		}

		case SDL_CONTROLLERBUTTONUP: {
			int player = event->cbutton.which;

			if (player >= 0 && player < PEMSA_PLAYER_COUNT) {
				int button = gamepadbutton_to_button(event->cbutton.button);

				if (button != -1) {
					this->state[player][button] = 0;
				}
			}

			break;
		}

		case SDL_MOUSEBUTTONDOWN: {
			if (event->button.button < 4) {
				SET_BIT(this->mouseState, event->button.button - 1, true);
			}

			break;
		}

		case SDL_MOUSEBUTTONUP: {
			if (event->button.button < 4) {
				SET_BIT(this->mouseState, event->button.button - 1, false);
			}

			break;
		}
	}
}

bool SdlInputBackend::isButtonDown(int i, int p) {
	if (i < 0 || i > PEMSA_BUTTON_COUNT - 1 || p < 0 || p > PEMSA_PLAYER_COUNT - 1) {
		return false;
	}

	return this->state[p][i] >= 1;
}

bool SdlInputBackend::isButtonPressed(int i, int p) {
	if (i < 0 || i > PEMSA_BUTTON_COUNT - 1 || p < 0 || p > PEMSA_PLAYER_COUNT - 1) {
		return false;
	}

	return this->state[p][i] == 2;
}

void SdlInputBackend::update() {
	for (int p = 0; p < PEMSA_PLAYER_COUNT; p++) {
		uint8_t* buttons = this->state[p];

		for (int i = 0; i < PEMSA_BUTTON_COUNT; i++) {
			uint8_t value = buttons[i];

			if (value == 0) {
				continue;
			}

			buttons[i] = (value) % 15 + 1;
		}
	}
}

int SdlInputBackend::getMouseX() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	SdlGraphicsBackend* graphics = (SdlGraphicsBackend*) this->emulator->getGraphicsModule()->getBackend();
	return (x - graphics->getOffsetX()) / graphics->getScale();
}

int SdlInputBackend::getMouseY() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	SdlGraphicsBackend* graphics = (SdlGraphicsBackend*) this->emulator->getGraphicsModule()->getBackend();
	return (y - graphics->getOffsetY()) / graphics->getScale();
}

int SdlInputBackend::getMouseMask() {
	return this->mouseState;
}

const char *SdlInputBackend::readKey() {
	if (this->isDown) {
		this->isDown = false;
		return SDL_GetScancodeName(this->lastKey);
	}

	return nullptr;
}

bool SdlInputBackend::hasKey() {
	return this->isDown;
}

void SdlInputBackend::reset() {
	for (int p = 0; p < PEMSA_PLAYER_COUNT; p++) {
		for (int i = 0; i < PEMSA_BUTTON_COUNT; i++) {
			this->state[p][i] = 0;
		}
	}

	this->mouseState = 0;
}