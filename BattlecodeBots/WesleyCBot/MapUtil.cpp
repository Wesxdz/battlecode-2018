#include "MapUtil.h"

std::vector<bc_MapLocation*> MapUtil::AllLocations(bc_PlanetMap* map)
{
	uintptr_t width = bc_PlanetMap_width_get(map);
	uintptr_t height = bc_PlanetMap_height_get(map);
	bc_Planet planet = bc_PlanetMap_planet_get(map);
	std::vector<bc_MapLocation*> locations;
	for (uintptr_t x = 0; x < width; x++) {
		for (uintptr_t y = 0; y < height; y++) {
			locations.push_back(new_bc_MapLocation(planet, x, y));
		}
	}
	return locations;
}

std::vector<bc_MapLocation*> MapUtil::FilteredLocations(bc_PlanetMap * map, std::function<bool(bc_MapLocation*)> shouldInclude)
{
	uintptr_t width = bc_PlanetMap_width_get(map);
	uintptr_t height = bc_PlanetMap_height_get(map);
	bc_Planet planet = bc_PlanetMap_planet_get(map);
	std::vector<bc_MapLocation*> locations;
	for (uintptr_t x = 0; x < width; x++) {
		for (uintptr_t y = 0; y < height; y++) {
			bc_MapLocation* loc = new_bc_MapLocation(planet, x, y);
			if (shouldInclude(loc)) {
				locations.push_back(loc);
			}
			else {
				delete_bc_MapLocation(loc);
			}
		}
	}
	return locations;
}