#include "Pathfind.h"

#include "Player.h"
#include <stdlib.h>
#include <algorithm>
#include "MapUtil.h"

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

bool Pathfind::MoveRandom(bc_Unit* robot)
{
	if (!bc_GameController_is_move_ready(Player::gc, bc_Unit_id(robot))) {
		return false;
	}
	for (auto direction : MapUtil::Adjacent) {
		int attempts = 0;
		for (int i = rand() % 8; attempts < 8; attempts++) {
			if (i == 8) { i = 0; }
			if (bc_GameController_can_move(Player::gc, bc_Unit_id(robot), static_cast<bc_Direction>(i))) {
				bc_GameController_move_robot(Player::gc, bc_Unit_id(robot), static_cast<bc_Direction>(i));
				return true;
			}
		}
	}
	return false;
}

bc_MapLocation* Pathfind::PickGreedy(bc_MapLocation* origin, bc_MapLocation* destination)
{
	auto moveable = Moveable(origin);
	if (moveable.size() == 0) return nullptr;
	bc_Direction ideal = bc_MapLocation_direction_to(origin, destination);
	std::sort(std::begin(moveable), std::end(moveable), [&origin, &ideal](bc_MapLocation* a, bc_MapLocation* b) {
		return abs(ideal - bc_MapLocation_direction_to(origin, a)) < abs(ideal - bc_MapLocation_direction_to(origin, b));
	});
	for (size_t i = 1; i < moveable.size(); i++) {
		delete_bc_MapLocation(moveable[i]);
	}
	if (bc_MapLocation_distance_squared_to(origin, destination) > 1 && bc_MapLocation_distance_squared_to(origin, destination) < 2) { // Knights
		if (moveable.size() > 0) return moveable[1];
	}
	return moveable[0];
}

bool Pathfind::MoveGreedy(bc_Unit* robot, bc_MapLocation* destination)
{
	if (!bc_GameController_is_move_ready(Player::gc, bc_Unit_id(robot))) {
		return false;
	}
	bc_Location* location = bc_Unit_location(robot);
	if (!bc_Location_is_on_map(location)) {
		delete_bc_Location(location);
		return false;
	}
	bc_MapLocation* mapLocation = bc_Location_map_location(location);
	bc_MapLocation* move = PickGreedy(mapLocation, destination);
	if (!move) {
		delete_bc_Location(location);
		delete_bc_MapLocation(mapLocation);
		return false;
	}
	bc_Direction moveDir = bc_MapLocation_direction_to(mapLocation, move);
	bc_GameController_move_robot(Player::gc, bc_Unit_id(robot), moveDir);
	delete_bc_Location(location);
	delete_bc_MapLocation(mapLocation);
	return true;
}

std::vector<bc_MapLocation*> Pathfind::AStarPath(bc_MapLocation* origin, bc_MapLocation* destination, std::function<int(bc_MapLocation*)> cost, int effort)
{
	return std::vector<bc_MapLocation*>();
}
