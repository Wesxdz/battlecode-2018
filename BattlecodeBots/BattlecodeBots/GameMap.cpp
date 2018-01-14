#include "GameMap.h"

GameMap::GameMap()
{
}

GameMap::~GameMap()
{
	delete_bc_GameMap(gm);
}

PlanetMap GameMap::Earth()
{
	PlanetMap earth;
	earth.self = bc_GameMap_earth_map_get(gm);
	return earth;
}

PlanetMap GameMap::Mars()
{
	PlanetMap mars;
	mars.self = bc_GameMap_mars_map_get(gm);
	return mars;
}
