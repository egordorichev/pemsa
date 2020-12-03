#ifndef PEMSA_INPUT_BACKEND_HPP
#define PEMSA_INPUT_BACKEND_HPP

#include "pemsa/pemsa_backend.hpp"

class PemsaInputBackend : public PemsaBackend {
	public:
		virtual bool isButtonDown(int i, int p) = 0;
		virtual bool isButtonPressed(int i, int p) = 0;
		virtual void update() = 0;

		virtual int getMouseX() = 0;
		virtual int getMouseY() = 0;
		virtual int getMouseMask() = 0;

		virtual const char* readKey() = 0;
		virtual bool hasKey() = 0;
		virtual void reset() = 0;

		virtual const char* getClipboardText() = 0;
};

#endif