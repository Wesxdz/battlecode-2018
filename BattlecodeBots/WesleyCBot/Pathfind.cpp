#include "Pathfind.h"

#include "Player.h"
#include <stdlib.h>
#include <algorithm>

std::vector<bc_MapLocation*> Pathfind::Neighbors(bc_MapLocation* start)
{
	std::vector<bc_MapLocation*> neighbors;
	bc_Planet planet = bc_MapLocation_planet_get(start);
	bc_PlanetMap* planetMap = Player::Map(bc_MapLocation_planet_get(start));
	for (int i = 0; i < 8; i++) { // Not including Center
		bc_MapLocation* neighbor = bc_MapLocation_add(start, static_cast<bc_Direction>(i));
		if (bc_PlanetMap_on_map(planetMap, neighbor)) {
			neighbors.push_back(neighbor);
		}
		else {
			delete_bc_MapLocation(neighbor);
		}
	}
	return neighbors;
}

std::vector<bc_MapLocation*> Pathfind::Moveable(bc_MapLocation* start)
{
	std::vector<bc_MapLocation*> neighbors;
	bc_Planet planet = bc_MapLocation_planet_get(start);
	bc_PlanetMap* planetMap = Player::Map(bc_MapLocation_planet_get(start));
	for (int i = 0; i < 8; i++) { // Not including Center
		bc_MapLocation* neighbor = bc_MapLocation_add(start, static_cast<bc_Direction>(i));
		if (
			bc_PlanetMap_on_map(planetMap, neighbor) &&
			(
				bc_GameController_can_sense_location(Player::gc, neighbor) && bc_GameController_is_occupiable(Player::gc, neighbor) ||
				bc_PlanetMap_is_passable_terrain_at(planetMap, neighbor)
				)
			) {
			neighbors.push_back(neighbor);
		}
		else {
			delete_bc_MapLocation(neighbor);
		}
	}
	return neighbors;
}

bc_MapLocation* Pathfind::PickRandom(std::vector<bc_MapLocation*>& locGrid)
{
	if (locGrid.size() == 0) return nullptr;
	return locGrid[rand() % locGrid.size()];
}

void Pathfind::MoveRandom(bc_Unit* robot)
{
	bc_Location* location = bc_Unit_location(robot);
	if (!bc_Location_is_on_map(location)) {
		delete_bc_Location(location);
		return;
	}
	bc_MapLocation* mapLocation = bc_Location_map_location(location);
	auto moveChoices = Pathfind::Moveable(mapLocation);
	bc_MapLocation* move = Pathfind::PickRandom(moveChoices);
	if (bc_GameController_is_move_ready(Player::gc, bc_Unit_id(robot))) {
		bc_GameController_move_robot(Player::gc, bc_Unit_id(robot), bc_MapLocation_direction_to(mapLocation, move));
	}
	for (bc_MapLocation* loc : moveChoices) {
		delete_bc_MapLocation(loc);
	}
	delete_bc_MapLocation(mapLocation);
	delete_bc_Location(location);
}

bc_MapLocation* Pathfind::PickGreedy(bc_MapLocation* origin, bc_MapLocation* destination)
{
	auto neighbors = Neighbors(origin);
	if (neighbors.size() == 0) return nullptr;
	bc_Direction ideal = bc_MapLocation_direction_to(origin, destination);
	std::sort(std::begin(neighbors), std::end(neighbors), [&origin, &ideal](bc_MapLocation* a, bc_MapLocation* b) {
		return abs(ideal - bc_MapLocation_direction_to(origin, a)) < abs(ideal - bc_MapLocation_direction_to(origin, b));
	});
	for (size_t i = 1; i < neighbors.size(); i++) {
		delete_bc_MapLocation(neighbors[i]);
	}
	if (bc_MapLocation_distance_squared_to(origin, destination) > 1 && bc_MapLocation_distance_squared_to(origin, destination) < 2) { // Knights
		if (neighbors.size() > 0) return neighbors[1];
	}
	return neighbors[0];
}

void Pathfind::MoveGreedy(bc_Unit* robot, bc_MapLocation* destination)
{
	bc_Location* location = bc_Unit_location(robot);
	if (!bc_Location_is_on_map(location)) {
		delete_bc_Location(location);
		return;
	}
	bc_MapLocation* mapLocation = bc_Location_map_location(location);
	bc_MapLocation* move = PickGreedy(mapLocation, destination);
	if (!move) return;
	bc_Direction moveDir = bc_MapLocation_direction_to(mapLocation, move);
	bc_GameController_move_robot(Player::gc, bc_Unit_id(robot), moveDir);
}

std::vector<bc_MapLocation*> Pathfind::AStarPath(bc_MapLocation* origin, bc_MapLocation* destination, std::function<int(bc_MapLocation*)> cost, int effort)
{
	return std::vector<bc_MapLocation*>();
}
