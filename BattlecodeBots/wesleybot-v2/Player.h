#ifndef PLAYER_H
#define PLAYER_H

#include "bc.h"

class Player
{
public:
	Player();
	~Player();
	static bc_GameController* gc;
	static bc_Team team;
	static bc_PlanetMap* earth;
	static bc_PlanetMap* mars;
	static bc_PlanetMap* Map(bc_Planet planet);
	static bc_Planet planet;
};

#endif

