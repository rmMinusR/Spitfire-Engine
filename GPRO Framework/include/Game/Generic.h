/*

This file does special logic for each #include.
The first time it gives the function prototype for the main method.
Any time after, it redefines the MAIN_FUNC macro to this game's main method.

Intended usage:

(at top)
#include "Generic.h"

(later)
if(gameSelected)
#include "Generic.h"
return MAIN_FUNC();

*/

// FUNCTION CALL HOOKS

#ifndef _game_generic
#define _game_generic

namespace game_generic {
	int game_main();
}

#else

#ifdef MAIN_FUNC
#undef MAIN_FUNC
#endif

#define MAIN_FUNC game_generic::game_main

#endif