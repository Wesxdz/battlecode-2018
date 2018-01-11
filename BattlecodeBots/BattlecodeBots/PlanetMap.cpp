#include "PlanetMap.h"

#include "GameMap.h"
#include "MapLocation.h"
#include "Log.h"

PlanetMap::PlanetMap()
{
}

PlanetMap::~PlanetMap()
{
	delete_bc_PlanetMap(self);
}

bc_Planet PlanetMap::Planet()
{
	return bc_PlanetMap_planet_get(self);
}

uintptr_t PlanetMap::Width()
{
	return bc_PlanetMap_width_get(self);
}

uintptr_t PlanetMap::Height()
{
	return bc_PlanetMap_height_get(self);
}

uint8_t PlanetMap::IsOnMap(MapLocation& location)
{
	return bc_PlanetMap_on_map(self, location.self);
}

uint8_t PlanetMap::IsPassableTerrain(MapLocation& location)
{
	auto passable = bc_PlanetMap_is_passable_terrain_at(self, location.self);
	CHECK_ERRORS();
	return passable;
}

uint32_t PlanetMap::InitialKarbonite(MapLocation& location)
{
	auto deposit = bc_PlanetMap_initial_karbonite_at(self, location.self);
	CHECK_ERRORS();
	return deposit;
}

std::vector<std::shared_ptr<units::Worker>> PlanetMap::InitialWorkers()
{
	auto workers = std::vector<std::shared_ptr<units::Worker>>();
	bc_VecUnit* bcWorkers;
	for (intptr_t i = 0; i < bc_VecUnit_len(bcWorkers); i++) {
		auto worker = std::make_shared<units::Worker>();
		worker->Init(bc_VecUnit_index(bcWorkers, i));
		workers.push_back(worker);
	}
	return workers;
}

std::vector<std::vector<MapLocation>> PlanetMap::Locations() {
	auto locations = std::vector<std::vector<MapLocation>>();
	auto planet = Planet();
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			locations[x][y] = MapLocation(planet, x, y);
		}
	}
	return locations;
}
