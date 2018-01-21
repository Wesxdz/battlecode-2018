#include "MapUtil.h"

#include "Section.h"
#include <iostream>
#include "GameController.h"
#include "PlanetMap.h"
#include "Region.h"

const uint32_t MapUtil::MIN_MAP_SIZE = 400;
const uint32_t MapUtil::MAX_MAP_SIZE = 2500;

uintptr_t MapUtil::EARTH_MAP_WIDTH = 20;
uintptr_t MapUtil::EARTH_MAP_HEIGHT = 20;
uintptr_t MapUtil::MARS_MAP_WIDTH = 20;
uintptr_t MapUtil::MARS_MAP_HEIGHT = 20;

std::vector<bc_MapLocation*> MapUtil::earthLocations;
std::vector<bc_MapLocation*> MapUtil::earthPassableLocations;
short* MapUtil::earthTerrainMap = nullptr;
std::vector<bc_MapLocation*> MapUtil::marsLocations;
std::vector<bc_MapLocation*> MapUtil::marsPassableLocations;
short* MapUtil::marsTerrainMap = nullptr;

std::vector<bc_MapLocation*> MapUtil::AllLocations(bc_PlanetMap* map)
{
	uintptr_t width = bc_PlanetMap_width_get(map);
	uintptr_t height = bc_PlanetMap_height_get(map);
	bc_Planet planet = bc_PlanetMap_planet_get(map);
	std::vector<bc_MapLocation*> locations;
	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			locations.push_back(new_bc_MapLocation(planet, x, y));
		}
	}
	return locations;
}

MapUtil::MapUtil()
{
	PlanetMap earth(GameController::PlanetMap(Earth));
	EARTH_MAP_WIDTH = earth.width;
	EARTH_MAP_HEIGHT = earth.height;

	earthTerrainMap = new short[EARTH_MAP_WIDTH * EARTH_MAP_HEIGHT];

	MapUtil::earthLocations = MapUtil::AllLocations(earth.self);
	MapUtil::earthPassableLocations = MapUtil::FilteredLocations(MapUtil::earthLocations, [](bc_MapLocation* location) {
		return bc_PlanetMap_is_passable_terrain_at(GameController::PlanetMap(Earth), location);
	});

	// We should probably incorporate this for loop into the previous call
	// Iterates over all locations and stores wether they are passable, that way we dont have to call it each time.
	std::cout << "Terrain Map\n";
	for (int y = 0; y < EARTH_MAP_HEIGHT; y++) {
		for (int x = 0; x < EARTH_MAP_WIDTH; x++) {
			short ID = static_cast<short>(y * EARTH_MAP_WIDTH + x);
			earthTerrainMap[ID] = bc_PlanetMap_is_passable_terrain_at(earth.self, earthLocations[ID]);
			std::cout << earthTerrainMap[ID] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;

	PlanetMap mars(GameController::PlanetMap(Mars));
	MARS_MAP_HEIGHT = mars.height;
	MARS_MAP_WIDTH = mars.width;

	marsTerrainMap = new short[MARS_MAP_WIDTH * MARS_MAP_HEIGHT];

	MapUtil::marsLocations = MapUtil::AllLocations(mars.self);
	MapUtil::marsPassableLocations = MapUtil::FilteredLocations(MapUtil::marsLocations, [](bc_MapLocation* location) {
		return bc_PlanetMap_is_passable_terrain_at(GameController::PlanetMap(Mars), location);
	});

	// We should probably incorporate this for loop into the previous call
	// Iterates over all locations and stores wether they are passable, that way we dont have to call it each time.
	for (int y = 0; y < MARS_MAP_HEIGHT; y++) {
		for (int x = 0; x < MARS_MAP_WIDTH; x++) {
			short ID = static_cast<short>(y * MARS_MAP_WIDTH + x);
			marsTerrainMap[ID] = bc_PlanetMap_is_passable_terrain_at(mars.self, marsLocations[ID]);
		}
	}

	Section::earthSections = Section::GenSections(MapUtil::earthPassableLocations);
	Section::marsSections = Section::GenSections(MapUtil::marsPassableLocations);

	// Call Regions ....
	//Region::GenRegions();
}

std::vector<bc_MapLocation*> MapUtil::FilteredLocations(std::vector<bc_MapLocation*>& potential, std::function<bool(bc_MapLocation*)> shouldInclude)
{
	std::vector<bc_MapLocation*> locations;
	for (bc_MapLocation* loc : potential) {
		if (shouldInclude(loc)) {
			locations.push_back(bc_MapLocation_clone(loc));
		}
	}
	return locations;
}

std::vector<bc_MapLocation*> MapUtil::FilteredLocations(bc_VecMapLocation* potential, std::function<bool(bc_MapLocation*)> shouldInclude)
{
	std::vector<bc_MapLocation*> locations;
	for (uintptr_t i = 0; i < bc_VecMapLocation_len(potential); i++) {
		bc_MapLocation* loc = bc_VecMapLocation_index(potential, i);
		if (shouldInclude(loc)) {
			locations.push_back(loc);
		}
		else {
			delete_bc_MapLocation(loc);
		}
	}
	return locations;
}
