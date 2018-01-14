#ifndef ROCKET_H 
#define ROCKET_H

#include "Structure.h"

#include "MapLocation.h"

namespace units {

	class Rocket : public Structure
	{
	public:
		Rocket(bc_Unit* unit);
		~Rocket();
		/*
		@return Whether the rocket has already been used.
		*/
		uint8_t IsUsed();
		/*
		@return The damage a rocket deals to adjacent units upon landing.
		*/
		uint32_t BlastDamage();
		/*
		@return The number of rounds the rocket travel time is reduced by compared to the travel time determined by the orbit of the planets.
		*/
		uint32_t ExtraSpeed();
		/*
		@return Whether the rocket can launch into space to the given destination. The rocket can launch if the it has never been used before. The destination is valid if it contains passable terrain on the other planet.
		*/
		uint8_t CanLaunch(MapLocation& location);
		/*
		@execute Launches the rocket into space, damaging the units adjacent to the takeoff location.
		*/
		void Launch(MapLocation& location);

	};

}


#endif