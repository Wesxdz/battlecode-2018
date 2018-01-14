#include "GameMap.h"
#include <iostream>

bc_GameMap* GameMap::gm = nullptr;

GameMap::GameMap()
{
	gm = bc_GameMap_test_map();
}

GameMap::~GameMap()
{
	delete_bc_GameMap(gm);
}

PlanetMap GameMap::Earth()
{
	auto planetID = bc_GameMap_earth_map_get(gm);
	PlanetMap earth(planetID);
	return earth;
}

PlanetMap GameMap::Mars()
{
	auto planetID = bc_GameMap_mars_map_get(gm);
	PlanetMap mars(planetID);
	return mars;
}
