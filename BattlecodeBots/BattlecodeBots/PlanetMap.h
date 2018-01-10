#pragma once

#include "bc.h"

#include <vector>

class GameMap;
class Unit;
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
private:
	PlanetMap();
public:
	/*
	Earth consists of passable squares, passable Karbonite deposits, and impassable water squares. 
	The number of Karbonite deposits on Earth is limited, and Earth will never replenish Karbonite over the course of the game. 
	The starting amount of Karbonite on a square on Earth is in the range [0, 50].
	At the start of round 750, Earth suffers a massive flood that destroys everything on the planet.
	@return Earth map
	*/
	static PlanetMap Earth(GameMap& map);
	/*
	Mars consists of passable squares, passable Karbonite deposits, and impassable rocky outcrops. 
	Mars begins with no Karbonite deposits. However, it is frequently bombarded by asteroids, which leave behind comparatively rich Karbonite deposits. 
	Mars is the only planet that has a renewable amount of Karbonite.
	@return Mars map
	*/
	static PlanetMap Mars(GameMap& map);
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

};

