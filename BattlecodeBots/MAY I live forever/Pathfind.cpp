#include "Pathfind.h"

#include "Constants.h"
#include "GameMap.h"
#include "PlanetMap.h"

#include <stdlib.h>
#include <algorithm>

std::vector<MapLocation> Pathfind::Neighbors(MapLocation & start)
{
	std::vector<MapLocation> neighbors;
	PlanetMap* planetMap = GameMap::OurPlanet();
	for (bc_Direction direction : constants::directions_adjacent) {
		MapLocation neighbor = MapLocation::Neighbor(start, direction);
		if (neighbor.IsValid()) neighbors.push_back(neighbor);
	}
	return neighbors;
}

std::vector<MapLocation> Pathfind::Moveable(MapLocation& start)
{
	std::vector<MapLocation> neighbors;
	PlanetMap* planetMap = GameMap::OurPlanet();
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

MapLocation Pathfind::PickGreedy(MapLocation& origin, MapLocation& destination)
{
	auto moveable = Moveable(origin);
	if (moveable.size() == 0) return nullptr;
	bc_Direction ideal = origin.DirectionTo(destination);
	std::sort(std::begin(moveable), std::end(moveable), [&origin, &ideal](MapLocation& a, MapLocation& b) {
		return abs(ideal - origin.DirectionTo(a)) < abs(ideal - origin.DirectionTo(b));
	});
	return moveable[0];
}

bool Pathfind::MoveGreedy(units::Robot& robot, MapLocation& destination)
{
	if (!robot.IsMoveReady() || !robot.Loc().IsOnMap()) return false;
	MapLocation location = robot.Loc().ToMapLocation();
	MapLocation move = PickGreedy(location, destination);
	robot.Move(location.DirectionTo(move));
	return true;
}
