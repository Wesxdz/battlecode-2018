#pragma once

#include "Robot.h"

#include "MapLocation.h"

namespace units {

	class Mage : public Robot
	{
	public:
		Mage();
		~Mage();
		/*
		@return Whether the mage is ready to blink. Tests whether the mage's ability heat is sufficiently low.
		*/
		uint8_t IsBlinkReady();
		/*
		@return Whether the mage can blink to the given location, without taking into account the mage's ability heat. Takes into account only the mage's ability range, the map terrain, positions of other units, and the edge of the game map.
		*/
		uint8_t CanBlink(MapLocation location);
		/*
		@execute Blinks the mage to the given location.
		*/
		void Blink(MapLocation location);
	};

}


