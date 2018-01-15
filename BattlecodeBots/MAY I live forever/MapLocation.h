#ifndef MAPLOCATION_H 
#define MAPLOCATION_H

#include "bc.h"

#include <vector>

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
	bc_MapLocation* self = nullptr;
public:
	MapLocation();
	MapLocation(bc_Planet planet, int32_t x, int32_t y);
	MapLocation(const MapLocation& other);
	MapLocation(bc_MapLocation* loc);
	~MapLocation();

	bool operator==(MapLocation& other);

	MapLocation operator=(const MapLocation& m);

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
	uint8_t IsOccupiable();
	/*
	@return The unit at the location
	*/
	bc_Unit* Occupant();

	/*
	(You can get invalid map locations by using methods like Neighbor and Translate)
	@return Whether this map location is on the map
	*/
	uint8_t IsValid();

	/*
	Is only false when the square contains impassable terrain (distinct from containing a building, for instance).
	@return Whether the location on the map contains passable terrain.
	*/
	uint8_t IsPassable();

	/*
	@return Units near the location within the given radius, inclusive, in distance squared. The units are within the vision range.
	*/
	std::vector<units::Unit> NearbyUnits(uint32_t radius_squared);
	/*
	@return Units near the location within the given radius, inclusive, in distance squared. The units are within the vision range. Additionally filters the units by team.
	*/
	std::vector<units::Unit> NearbyUnits(uint32_t radius_squared, bc_Team team);
	/*
	@return Units near the location within the given radius, inclusive, in distance squared. The units are within the vision range. Additionally filters the units by unit type.
	*/
	template<class T>
	std::vector<T> NearbyUnits(uint32_t radius_squared, bc_UnitType type);

	static std::vector<MapLocation> NearbyLocations(MapLocation& location, uint32_t radius_squared);
};

#endif
