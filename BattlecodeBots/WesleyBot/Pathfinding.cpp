#include "Pathfinding.h"

#include "bc.h"
#include "GameController.h"
#include "GameMap.h"

#include <algorithm>
#include <map>

std::vector<MapLocation> Pathfinding::Neighbors(MapLocation& start)
{
	std::vector<MapLocation> adjacentLocations;
	for (int i = 0; i < 8; i++) { // Not including Center
		auto neighbor = start.Neighbor(start, static_cast<bc_Direction>(i));
		if (neighbor.IsValid()) {
			adjacentLocations.push_back(neighbor);
		}
	}
	return adjacentLocations;
}

std::vector<MapLocation> Pathfinding::Moveable(MapLocation& start)
{
	std::vector<MapLocation> moveable;
	for (int i = 0; i < 8; i++) { // Not including Center
		auto neighbor = MapLocation::Neighbor(start, static_cast<bc_Direction>(i));
		if (neighbor.IsValid() && neighbor.IsPassable()) {
			if (neighbor.IsVisible()) {
				if (neighbor.IsOccupiable()) {
					moveable.push_back(neighbor);
				}
			}
			else {
				moveable.push_back(neighbor);
			}
		}
	}
	return moveable;
}

std::vector<MapLocation> Pathfinding::Deposits(MapLocation& start)
{
	std::vector<MapLocation> deposits;
	for (int i = 0; i < 9; i++) {
		auto potentialDeposit = MapLocation::Neighbor(start, static_cast<bc_Direction>(i));
		if (potentialDeposit.IsValid() && potentialDeposit.Karbonite() > 0) {
			deposits.push_back(potentialDeposit);
		}
	}
	return deposits;
}

MapLocation Pathfinding::PickRandom(std::vector<MapLocation>& locations)
{
	return locations[rand() % locations.size()];
}

bc_Direction Pathfinding::PickGreedy(MapLocation& start, MapLocation& end)
{
	if (start.IsAdjacentTo(end)) return start.DirectionTo(end);
	bc_Direction directionToEnd = start.DirectionTo(end);
	MapLocation best = MapLocation::Neighbor(start, directionToEnd);
	if (best.IsValid() && best.IsVisible() && best.IsOccupiable()) {
		return directionToEnd;
	}
	bc_Direction nextRight = bc_Direction_rotate_right(directionToEnd);
	bc_Direction nextLeft = bc_Direction_rotate_left(directionToEnd);
	best = MapLocation::Neighbor(start, nextRight);
	if (best.IsValid() && best.IsVisible() && best.IsOccupiable()) {
		return nextRight;
	}
	best = MapLocation::Neighbor(start, nextLeft);
	if (best.IsValid() && best.IsVisible() && best.IsOccupiable()) {
		return nextLeft;
	}
	nextRight = bc_Direction_rotate_right(nextRight);
	nextLeft = bc_Direction_rotate_left(nextLeft);
	best = MapLocation::Neighbor(start, nextRight);
	if (best.IsValid() && best.IsVisible() && best.IsOccupiable()) {
		return nextRight;
	}
	best = MapLocation::Neighbor(start, nextLeft);
	if (best.IsValid() && best.IsVisible() && best.IsOccupiable()) {
		return nextLeft;
	}
	nextRight = bc_Direction_rotate_right(nextRight);
	nextLeft = bc_Direction_rotate_left(nextLeft);
	best = MapLocation::Neighbor(start, nextRight);
	if (best.IsValid() && best.IsVisible() && best.IsOccupiable()) {
		return nextRight;
	}
	best = MapLocation::Neighbor(start, nextLeft);
	if (best.IsValid() && best.IsVisible() && best.IsOccupiable()) {
		return nextLeft;
	}
	// Doesn't move backwards
	return bc_Direction::Center;
}

MapLocation Pathfinding::PickGreedyFancy(MapLocation start, MapLocation end, std::map<int, MapLocation>* discouraged)
{
	return start;
}

std::vector<MapLocation> Pathfinding::FindPath(MapLocation start, MapLocation end, std::function<int(MapLocation)> cost)
{
	//std::vector<MapLocation> closedSet;
	//std::vector<MapLocation> openSet;
	//std::map<
	return std::vector<MapLocation>();
}
