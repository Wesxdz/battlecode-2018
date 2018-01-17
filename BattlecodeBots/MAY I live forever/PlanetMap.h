#ifndef PLANETMAP_H 
#define PLANETMAP_H

#include "bc.h"
#include "Worker.h"

#include <vector>

class GameMap;
class MapLocation;

/*
In Escape to Mars, all game events occur in discrete 2 dimensional grids called maps. 
A map consists of a rectangular space of size ranging from 20x20 to 50x50. 
Each point has x and y coordinates representing its location. The bottom left corner (origin) always has the coordinates (0, 0). 
Coordinates increase to the North (up) and the East (right).
*/
class PlanetMap
{
public:
	bc_PlanetMap* self;
	bc_Planet planetType;
	uintptr_t width;
	uintptr_t height;
public:
	PlanetMap();
	PlanetMap(bc_PlanetMap* self);
	PlanetMap(const PlanetMap& other);
	~PlanetMap();
	/*
	@return The planet of the map
	*/
	bc_Planet Planet();
	/*
	@return The width of this map, in squares. Must be in the range [MAP_WIDTH_MIN, MAP_WIDTH_MAX], inclusive.
	*/
	uintptr_t Width();
	/*
	@return The height of this map, in squares. Must be in the range [MAP_HEIGHT_MIN, MAP_HEIGHT_MAX], inclusive
	*/
	uintptr_t Height();

	/*
	@return Whether a location is contained on the map
	*/
	uint8_t IsOnMap(MapLocation& location);

	/*
	Is only false when the square contains impassable terrain (distinct from containing a building, for instance).
	@return Whether the location on the map contains passable terrain.
	*/
	uint8_t IsPassableTerrain(MapLocation& location);

	/*
	@return The amount of Karbonite initially deposited at the given location.
	*/
	uint32_t InitialKarbonite(MapLocation& location);

	/*
	@return The initial units on the map. Each team starts with 1 to 3 Workers on Earth.
	*/
	std::vector<units::Worker> InitialWorkers();

	/*
	@return All locations on planet
	*/
	std::vector<MapLocation> Locations();

};

#endif