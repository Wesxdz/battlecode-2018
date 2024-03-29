#include "PlanetMap.h"

#include "MapLocation.h"
#include "Log.h"
#include "GameController.h"


PlanetMap::~PlanetMap()
{
}

PlanetMap::PlanetMap(bc_PlanetMap* self)
{
	this->self = self;
	planetType = bc_PlanetMap_planet_get(self);
	width = bc_PlanetMap_width_get(self);
	height = bc_PlanetMap_height_get(self);
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
	 return location.X() >= 0 && location.X() < Width() && location.Y() >= 0 && location.Y() < Height();
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

std::vector<units::Worker> PlanetMap::InitialWorkers()
{
	auto workers = std::vector<units::Worker>();
	bc_VecUnit* bcWorkers = bc_PlanetMap_initial_units_get(self);
	for (uintptr_t i = 0; i < bc_VecUnit_len(bcWorkers); i++) {
		units::Worker worker(bc_VecUnit_index(bcWorkers, i));
		workers.push_back(worker);
	}
	delete_bc_VecUnit(bcWorkers);
	return workers;
}

std::vector<MapLocation> PlanetMap::Locations() {
	auto locations = std::vector<MapLocation>();
	auto planet = Planet();
	for (uint32_t x = 0; x < Width(); x++) {
		for (uint32_t y = 0; y < Height(); y++) {
			locations.push_back(MapLocation(planet, x, y));
		}
	}
	return locations;
}
