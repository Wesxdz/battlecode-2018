#include "GameMap.h"
#include <iostream>

#include "GameController.h"

bc_GameMap* GameMap::gm = nullptr;
PlanetMap GameMap::earth;
PlanetMap GameMap::mars;

GameMap::GameMap()
{
	gm = bc_GameMap_test_map();
	earth = PlanetMap(bc_GameMap_earth_map_get(gm));
	mars = PlanetMap(bc_GameMap_mars_map_get(gm));
}

GameMap::~GameMap()
{
	delete_bc_GameMap(gm);
}

PlanetMap* GameMap::OurPlanet() {
	return (GameController::Planet() == bc_Planet::Earth ? &earth : &mars);
}

PlanetMap GameMap::Earth()
{
	//auto planetID = bc_GameMap_earth_map_get(gm);
	//PlanetMap earth(planetID);
	//PlanetMap p(;
	//return PlanetMap(bc_GameMap_earth_map_get(gm));
	return earth;
}

PlanetMap GameMap::Mars()
{
	//auto planetID = bc_GameMap_mars_map_get(gm);
	//PlanetMap mars(planetID);
	return mars;
}
