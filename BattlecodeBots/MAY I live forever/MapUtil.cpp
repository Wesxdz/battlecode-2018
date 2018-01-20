#include "MapUtil.h"

#include "Section.h"
#include <iostream>
#include "GameController.h"
#include "PlanetMap.h"

const uint32_t MapUtil::MIN_MAP_SIZE = 400;
const uint32_t MapUtil::MAX_MAP_SIZE = 2500;

uint32_t MapUtil::EARTH_MAP_WIDTH = 20;
uint32_t MapUtil::EARTH_MAP_HEIGHT = 20;
uint32_t MapUtil::MARS_MAP_WIDTH = 20;
uint32_t MapUtil::MARS_MAP_HEIGHT = 20;

std::vector<bc_MapLocation*> MapUtil::earthLocations;
std::vector<bc_MapLocation*> MapUtil::earthPassableLocations;
std::vector<bc_MapLocation*> MapUtil::marsLocations;
std::vector<bc_MapLocation*> MapUtil::marsPassableLocations;

std::vector<bc_MapLocation*> MapUtil::AllLocations(bc_PlanetMap* map)
{
	uintptr_t width = bc_PlanetMap_width_get(map);
	uintptr_t height = bc_PlanetMap_height_get(map);
	bc_Planet planet = bc_PlanetMap_planet_get(map);
	std::vector<bc_MapLocation*> locations;
	for (uintptr_t y = 0; y < height; y++) {
		for (uintptr_t x = 0; x < width; x++) {
			locations.push_back(new_bc_MapLocation(planet, x, y));
		}
	}
	return locations;
}

MapUtil::MapUtil()
{
	PlanetMap earth(GameController::PlanetMap(Earth));
	MapUtil::earthLocations = MapUtil::AllLocations(earth.self);
	MapUtil::earthPassableLocations = MapUtil::FilteredLocations(MapUtil::earthLocations, [](bc_MapLocation* location) {
		return bc_PlanetMap_is_passable_terrain_at(GameController::PlanetMap(Earth), location);
	});

	EARTH_MAP_WIDTH = earth.width;
	EARTH_MAP_HEIGHT = earth.height;

	PlanetMap mars(GameController::PlanetMap(Mars));
	MapUtil::marsLocations = MapUtil::AllLocations(mars.self);
	MapUtil::marsPassableLocations = MapUtil::FilteredLocations(MapUtil::marsLocations, [](bc_MapLocation* location) {
		return bc_PlanetMap_is_passable_terrain_at(GameController::PlanetMap(Mars), location);
	});

	MARS_MAP_HEIGHT = mars.height;
	MARS_MAP_WIDTH = mars.width;

	Section::earthSections = Section::GenSections(MapUtil::earthPassableLocations);
	Section::marsSections = Section::GenSections(MapUtil::marsPassableLocations);
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
