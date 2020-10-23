#ifndef PEMSA_MODULE_HPP
#define PEMSA_MODULE_HPP

#include "pemsa/pemsa_predefines.hpp"
#include "lua.hpp"

class PemsaModule {
	public:
		PemsaModule(PemsaEmulator* emulator);

		virtual void defineApi(lua_State* state);
		virtual void init();
		virtual void update();
		virtual void onCartridgeLoaded();
	protected:
		PemsaEmulator* emulator;
};

#endif