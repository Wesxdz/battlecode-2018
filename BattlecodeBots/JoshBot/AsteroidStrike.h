#ifndef ASTEROIDSTRIKE_H 
#define ASTEROIDSTRIKE_H

#include "bc.h"
#include "MapLocation.h"

class AsteroidStrike
{
public:
	bc_AsteroidStrike* self;
public:
	AsteroidStrike(bc_AsteroidStrike* strike);
	~AsteroidStrike();
	/*
	@return Karbonite contained in the asteroid
	*/
	uint32_t Karbonite();
	/*
	@return Landing location of the asteroid
	*/
	MapLocation Loc();
};

#endif