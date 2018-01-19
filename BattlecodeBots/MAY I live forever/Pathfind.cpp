#include "Pathfind.h"

#include "Constants.h"
#include "PlanetMap.h"
#include "GameController.h"

#include <stdlib.h>
#include <algorithm>

std::vector<MapLocation> Pathfind::Neighbors(MapLocation & start)
{
	std::vector<MapLocation> neighbors;
	bc_PlanetMap* planetMap = GameController::PlanetMap(start.Planet());
	for (bc_Direction direction : constants::directions_adjacent) {
		MapLocation neighbor = MapLocation::Neighbor(start, direction);
		if (neighbor.IsValid()) neighbors.push_back(neighbor);
	}
	return neighbors;
}

std::vector<MapLocation> Pathfind::Moveable(MapLocation& start)
{
	std::vector<MapLocation> neighbors;
	bc_PlanetMap* planetMap = GameController::PlanetMap(start.Planet());
	for (bc_Direction direction : constants::directions_adjacent) {
		MapLocation neighbor = MapLocation::Neighbor(start, direction);
		if (neighbor.IsValid()) {
			if (neighbor.IsVisible()) {
				if (neighbor.IsOccupiable()) {
					neighbors.push_back(neighbor);
				}
			}
			else if (neighbor.IsPassable()) {
				neighbors.push_back(neighbor);
			}
		}
	}
	return neighbors;
}

MapLocation Pathfind::PickRandom(std::vector<MapLocation>& locations)
{
	if (locations.size() == 0) return nullptr;
	return locations[rand() % locations.size()];
}

bool Pathfind::MoveRandom(units::Robot& robot)
{
	if (!robot.IsMoveReady()) return false;
	std::array<bc_Direction, 8> adjacent = constants::directions_adjacent;
	std::random_shuffle(std::begin(adjacent), std::end(adjacent));
	for (bc_Direction direction : adjacent) {
		if (robot.CanMove(direction)) {
			robot.Move(direction);
			return true;
		}
	}
	return false;
}

bool Pathfind::MoveFuzzy(units::Robot & robot, bc_Direction direction)
{
	if (robot.CanMove(direction)) {
		robot.Move(direction);
		return true;
	}
	bc_Direction right1 = bc_Direction_rotate_right(direction);
	if (robot.CanMove(right1)) {
		robot.Move(right1);
		return true;
	}
	bc_Direction left1 = bc_Direction_rotate_left(direction);
	if (robot.CanMove(left1)) {
		robot.Move(left1);
		return true;
	}
	bc_Direction right2 = bc_Direction_rotate_right(right1);
	if (robot.CanMove(right2)) {
		robot.Move(right2);
		return true;
	}
	bc_Direction left2 = bc_Direction_rotate_left(left1);
	if (robot.CanMove(left2)) {
		robot.Move(left2);
		return true;
	}
	return false;
}
