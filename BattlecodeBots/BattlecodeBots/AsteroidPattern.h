#pragma once

#include "bc.h"

class GameMap;

/*
Every [10, 20] rounds, a small asteroid strikes the surface at a single square. Asteroids carry amounts of Karbonite in the range [20, 100]. 
A Karbonite deposit forms at the location of the asteroid strike. If the location is occupied by a unit, the Karbonite settles neatly underneath the unit.
*/
class AsteroidPattern
{
public:
	bc_AsteroidPattern* self;
public:
	AsteroidPattern(GameMap& map);
	~AsteroidPattern();
};

