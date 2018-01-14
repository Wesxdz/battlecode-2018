#ifndef GAMEMAP_H 
#define GAMEMAP_H

#include "bc.h"

#include "PlanetMap.h"

class GameMap
{
public:
	static bc_GameMap* gm;
public:
	GameMap();
	~GameMap();
	/*
	Earth consists of passable squares, passable Karbonite deposits, and impassable water squares.
	The number of Karbonite deposits on Earth is limited, and Earth will never replenish Karbonite over the course of the game.
	The starting amount of Karbonite on a square on Earth is in the range [0, 50].
	At the start of round 750, Earth suffers a massive flood that destroys everything on the planet.
	@return Earth map
	*/
	static PlanetMap Earth();
	/*
	Mars consists of passable squares, passable Karbonite deposits, and impassable rocky outcrops.
	Mars begins with no Karbonite deposits. However, it is frequently bombarded by asteroids, which leave behind comparatively rich Karbonite deposits.
	Mars is the only planet that has a renewable amount of Karbonite.
	@return Mars map
	*/
	static PlanetMap Mars();

};

#endif