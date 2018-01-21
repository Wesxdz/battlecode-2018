#ifndef REGION_H
#define REGION_H

#include <vector>
#include "bc.h"

class Region {
public:
	// Is Chokepoint
	// Is on Mars
	// ID?
	std::vector<bc_MapLocation*> locations; // Curr Locations in this location
	std::vector<Region*> connectedRegions; // Other regions it is connected to. Typically via chokepoints

	static std::vector<Region> regions;
public:
	~Region();

	// Location is in Region?
	// Pathfinding from region to region?

	static void GenRegions(std::vector<bc_MapLocation*>& passables);

};

// Choke points are areas that provide easy, but limited access to other areas.
// As a result, we shold check the impassable terrain and passable terrain.
// 

#endif // !REGION_H
