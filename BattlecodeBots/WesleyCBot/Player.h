#ifndef PLAYER_H
#define PLAYER_H

#include "bc.h"

class Player
{
public:
	static void Init();
	static bc_GameController* gc;
	static bc_PlanetMap* earth;
	static bc_PlanetMap* mars;
	static bc_PlanetMap* Map(bc_Planet planet);
	static void Shutdown();
};

#endif

