#ifndef MAPUTIL_H
#define MAP_UTIL_H

#include "bc.h"
#include <vector>
#include <functional>

class MapUtil
{
public:
	static const uint32_t MIN_MAP_SIZE;
	static const uint32_t MAX_MAP_SIZE;
	static std::vector<bc_MapLocation*> earthLocations;
	static std::vector<bc_MapLocation*> earthPassableLocations;
	static std::vector<bc_MapLocation*> marsLocations;
	static std::vector<bc_MapLocation*> marsPassableLocations;
	static std::vector<bc_Direction> Adjacent;
	static std::vector<bc_MapLocation*> FilteredLocations(std::vector<bc_MapLocation*>& potential, std::function<bool(bc_MapLocation*)> shouldInclude);
	static std::vector<bc_MapLocation*> FilteredLocations(bc_VecMapLocation* potential, std::function<bool(bc_MapLocation*)> shouldInclude);
	static std::vector<bc_MapLocation*> AllLocations(bc_PlanetMap* map);
	//std::vector<bc_MapLocation*> Reflection(bc_MapLocation*)
};

#endif

