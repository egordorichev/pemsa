#ifndef SDL_GRAPHICS_BACKEND_HPP
#define SDL_GRAPHICS_BACKEND_HPP

#include "pemsa/graphics/pemsa_graphics_backend.hpp"

class SdlGraphicsBackend : public PemsaGraphicsBackend {
	public:
		void createSurface() override;
		void flip() override;
};

#endif