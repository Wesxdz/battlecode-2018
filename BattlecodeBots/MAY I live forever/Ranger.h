#ifndef RANGER_H 
#define RANGER_H

#include "Robot.h"

#include "MapLocation.h"

namespace units {

	class Ranger : public Robot
	{
	public:
		Ranger(bc_Unit* unit);
		~Ranger();
		/*
		@return The range within a ranger cannot attack.
		*/
		uint32_t AttackRangeMin();
		/*
		@return The countdown for ranger's snipe, which is the number of turns that must pass before the snipe is executed.
		*/
		uint32_t SnipeSpeed();
		/*
		@return Whether the ranger is sniping.
		*/
		uint8_t IsSniping();
		/*
		@error NullValue - the ranger is not sniping.
		@return The countdown for ranger's snipe.
		*/
		MapLocation SnipeTarget();
		/*
		Whether the ranger is ready to begin snipe. Tests whether the ranger's ability heat is sufficiently low.
		*/
		uint8_t IsSnipeReady();
		/*
		@return Whether the ranger can begin to snipe the given location, without taking into account the ranger's ability heat. Takes into account only the target location and the unit's type and unlocked abilities.
		*/
		uint8_t CanBeginSnipe(MapLocation& location);
		/*
		@execute Begins the countdown to snipe a given location. Maximizes the units attack and movement heats until the ranger has sniped. The ranger may begin the countdown at any time, including resetting the countdown to snipe a different location.
		*/
		void BeginSnipe(MapLocation& location);
	};

}


#endif