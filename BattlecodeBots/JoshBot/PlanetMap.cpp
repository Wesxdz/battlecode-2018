#include "PlanetMap.h"

#include "GameMap.h"
#include "MapLocation.h"

PlanetMap::PlanetMap()
{
	self = new_bc_PlanetMap();
	planetType = bc_PlanetMap_planet_get(self);
	width = bc_PlanetMap_width_get(self);
	height = bc_PlanetMap_height_get(self);
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

uint8_t PlanetMap::IsPassableTerrain(MapLocation & location)
{
	return bc_PlanetMap_is_passable_terrain_at(self, location.self);
}

uint32_t PlanetMap::InitialKarbonite(MapLocation& location)
{
	return bc_PlanetMap_initial_karbonite_at(self, location.self);
}
