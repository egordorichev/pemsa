#ifndef PEMSA_CARTRIDGE_MODULE_HPP
#define PEMSA_CARTRIDGE_MODULE_HPP

#include "pemsa/pemsa_module.hpp"
#include "pemsa/cart/pemsa_cartridge.hpp"

#include <condition_variable>
#include <thread>
#include <future>

class PemsaCartridgeModule : public PemsaModule {
	public:
		PemsaCartridgeModule(PemsaEmulator* emulator);
		~PemsaCartridgeModule();

		void update(double dt) override;
		bool load(const char* path);

		PemsaCartridge* getCart();
		std::condition_variable* getLock();
		std::mutex* getMutex();
	private:
		PemsaCartridge* cart;

		std::condition_variable lock;
		std::mutex mutex;
		std::thread* gameThread;
		bool threadRunning;

		void gameLoop();
		void cleanupCart();

		void callIfExists(const char* function);
		bool globalExists(const char* name);
		void reportLuaError();
};

void pemsa_open_input_api(PemsaEmulator* machine, lua_State* state);

#endif