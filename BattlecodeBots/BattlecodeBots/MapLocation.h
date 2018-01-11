#pragma once

#include "bc.h"

#include <memory>

namespace units {
	class Unit;
}

/*
A MapLocation represents a concrete space on a given planet. 
It has x and y coordinates, in addition to the planet itself, as attributes.
*/
class MapLocation
{
public:
	bc_MapLocation* self;
public:
	MapLocation(bc_Planet planet, int32_t x, int32_t y);
	MapLocation(bc_MapLocation* loc);
	~MapLocation();
	bc_Planet Planet();
	/*
	@return The x coordinate of the map location.
	*/
	int32_t X();
	/*
	@return The y coordinate of the map location.
	*/
	int32_t Y();
	/*
	@return The location one square from #origin in the given direction.
	*/
	static MapLocation Neighbor(MapLocation& origin, bc_Direction direction);
	/*
	@return The location translated from #origin by #dx and #dy
	*/
	static MapLocation Translate(MapLocation& origin, int32_t dx, int32_t dy);
	/*
	@return The square of the distance from this to #location.
	*/
	uint32_t DistanceTo(MapLocation& location);
	/*
	@return The Direction from this location to the specified location.
	*/
	bc_Direction DirectionTo(MapLocation& location);
	/*
	@return This location is adjacent to #location.
	*/
	uint8_t IsAdjacentTo(MapLocation& location);
	/*
	@return This location is within the distance squared range of #location, inclusive.
	*/
	uint8_t IsWithinRange(MapLocation& location, uint32_t range);
	/*
	@return The current amount of karbonite at this location
	*/
	uint32_t Karbonite();
	/*
	@return Whether the location is on the map and within the vision range.
	*/
	uint8_t IsVisible();
	/*
	@error LocationNotVisible - the location is outside the vision range.
	@return Whether the location is clear for a unit to occupy, either by movement or by construction.
	*/
	uint8_t Occupiable();
	/*
	@return The unit at the location
	*/
	std::shared_ptr<units::Unit> Occupant();
};

