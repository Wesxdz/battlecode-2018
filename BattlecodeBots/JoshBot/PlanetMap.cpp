#include "PlanetMap.h"

#include "GameController.h"
#include "GameMap.h"
#include "MapLocation.h"

PlanetMap::PlanetMap() 
{
	planetType = (bc_GameController_planet(GameController::gc));
	if(planetType == bc_Planet::Earth) {
		self = bc_GameMap_earth_map_get(GameMap::gm);
	} else{
		self = bc_GameMap_mars_map_get(GameMap::gm);		
	}
	
	width = bc_PlanetMap_width_get(self);
	height = bc_PlanetMap_height_get(self);
}

PlanetMap::PlanetMap(bc_Planet type) {
	if (type == bc_Planet::Earth) {
		self = bc_GameMap_earth_map_get(GameMap::gm);
	} else{
		self = bc_GameMap_mars_map_get(GameMap::gm);
	}
	
	planetType = type;
	width = bc_PlanetMap_width_get(self);
	height = bc_PlanetMap_height_get(self);
}

PlanetMap::PlanetMap(bc_PlanetMap* self)
{
	this->self = self;
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
	planetType = bc_PlanetMap_planet_get(self);
	return planetType;
}

uintptr_t PlanetMap::Width()
{
	width = bc_PlanetMap_width_get(self);
	return width;
}

uintptr_t PlanetMap::Height()
{
	height = bc_PlanetMap_height_get(self); 
	return height;
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
