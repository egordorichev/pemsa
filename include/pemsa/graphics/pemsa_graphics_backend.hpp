#ifndef PEMSA_GRAPHICS_BACKEND_HPP
#define PEMSA_GRAPHICS_BACKEND_HPP

#include "pemsa/pemsa_backend.hpp"

class PemsaGraphicsBackend : public PemsaBackend {
	public:
		virtual void createSurface() = 0;
		virtual void flip() = 0;
		virtual int getFps() = 0;
};

#endif