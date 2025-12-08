#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include "GameApp.h"


int main(int, const char*const*)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(7657); // Uncomment to break at specific memory allocation number

	GameApp game;
	bool interactive;
	static const physx::PxU32 frameCount;

#ifndef OFFLINE_EXECUTION 
	interactive = true;
#else
	interactive = false;
 	frameCount = 100;
#endif

	try {
		std::cout << "Starting application...\n";
		if (!interactive) {
			std::cout << "Running in OFFLINE mode for " << frameCount << " frames.\n";
			game.runOffline(frameCount);
		}
		else {
			std::cout << "Running in INTERACTIVE mode.\n";
			game.runInteractive();
		}
		std::cout << "Closing application...\n";
	} catch (std::exception& e) {
		std::cout << "\x1b[91;1m[FATAL]\x1b[0m " << e.what() << '\n';
		// std::cin.ignore(INT_MAX, '\n');
	} catch (...) {
		std::cout << "\x1b[91;1m[FATAL]\x1b[0m Closing application...\n";
		// std::cin.ignore(INT_MAX, '\n');
	};

	return 0;
}
