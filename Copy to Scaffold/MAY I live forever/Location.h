#ifndef LOCATION_H 
#define LOCATION_H

#include "bc.h"
#include "MapLocation.h"

#include <memory>

class Structure;

/*
A Location represents the location of a robot. 
Whenever a robot is on a map, this object maps directly to a MapLocation object. 
However, this is not always the case! 
A Location may also represent a point in space (as in the case of a rocket traveling to Mars), or a space in a structure’s garrison. 
Methods can be used to determine, more concretely, what a Location represents.
*/
class Location
{
public:
	bc_Location* self;
public:
	Location(); // Create and set #self in Unit
	~Location();
	/*
	@return this location is on a PlanetMap
	*/
	uint8_t OnMap();
	/*
	@return Whether the unit is on the map and on #planet
	*/
	uint8_t OnPlanet(bc_Planet planet);
	/*
	@error UnitNotOnMap
	@return the map location of the unit
	*/
	MapLocation ToMapLocation();
	/*
	@return Whether the unit is in a garrison.
	*/
	uint8_t IsInGarrison();
	/*
	@error UnitNotInGarrison
	@return The structure whose garrison the unit is in.
	*/
	std::shared_ptr<Structure> GarrisonStructure();
	/*
	@return Whether the unit is in space.
	*/
	uint8_t IsInSpace();

	// TODO
	// Adjacent to
	// Within range
};

#endif