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

void GameMap::Update()
{
	delete_bc_GameMap(gm);
	gm = bc_GameMap_test_map();
	earth = PlanetMap(bc_GameMap_earth_map_get(gm));
	mars = PlanetMap(bc_GameMap_mars_map_get(gm));
}

PlanetMap* GameMap::OurPlanet() {
	return (GameController::Planet() == bc_Planet::Earth ? &earth : &mars);
}

PlanetMap* GameMap::OtherPlanet() {
	return (GameController::Planet() == bc_Planet::Earth ? &mars : &earth);
}

PlanetMap GameMap::Earth()
{
	return earth;
}

PlanetMap GameMap::Mars()
{
	return mars;
}
