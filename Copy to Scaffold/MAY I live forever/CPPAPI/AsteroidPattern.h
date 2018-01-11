#pragma once

#include "bc.h"
#include "AsteroidStrike.h"

/*
Every [10, 20] rounds, a small asteroid strikes the surface at a single square. Asteroids carry amounts of Karbonite in the range [20, 100]. 
A Karbonite deposit forms at the location of the asteroid strike. If the location is occupied by a unit, the Karbonite settles neatly underneath the unit.
*/
class AsteroidPattern
{
public:
	bc_AsteroidPattern* self;
public:
	AsteroidPattern();
	~AsteroidPattern();
	/*
	@return Whether there is an asteroid strike at the given round.
	*/
	uint8_t WillAsteroidStrike(uint32_t round);
	/*
	@error NullValue - There is no asteroid strike at this round.
	@return The asteroid strike at the given round.
	*/
	AsteroidStrike Strike(uint32_t round);

};

