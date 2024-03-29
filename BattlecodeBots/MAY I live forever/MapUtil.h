#ifndef MAPUTIL_H
#define MAP_UTIL_H

#include "bc.h"
#include <vector>
#include <functional>

class MapUtil
{
public:
	MapUtil();
	static const uint32_t MIN_MAP_SIZE;
	static const uint32_t MAX_MAP_SIZE;

	static uintptr_t EARTH_MAP_WIDTH;
	static uintptr_t EARTH_MAP_HEIGHT;
	static uintptr_t MARS_MAP_WIDTH;
	static uintptr_t MARS_MAP_HEIGHT;

	static std::vector<bc_MapLocation*> earthLocations;
	static std::vector<bc_MapLocation*> earthPassableLocations;
	static short* earthTerrainMap;

	static std::vector<bc_MapLocation*> marsLocations;
	static std::vector<bc_MapLocation*> marsPassableLocations;
	static short* marsTerrainMap;

	static std::vector<bc_MapLocation*> FilteredLocations(std::vector<bc_MapLocation*>& potential, std::function<bool(bc_MapLocation*)> shouldInclude);
	static std::vector<bc_MapLocation*> FilteredLocations(bc_VecMapLocation* potential, std::function<bool(bc_MapLocation*)> shouldInclude);
	static std::vector<bc_MapLocation*> AllLocations(bc_PlanetMap* map);
	//std::vector<bc_MapLocation*> Reflection(bc_MapLocation*)
};

#endif

