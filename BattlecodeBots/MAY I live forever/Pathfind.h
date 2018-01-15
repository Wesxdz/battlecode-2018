#ifndef PATHFIND_H
#define PATHFIND_H

#include "bc.h"
#include <vector>
#include <functional>
#include "MapLocation.h"
#include "Robot.h"

class Pathfind
{
public:
	static std::vector<MapLocation> Neighbors(MapLocation& start);
	static std::vector<MapLocation> Moveable(MapLocation& start);
	static MapLocation PickRandom(std::vector<MapLocation>& locations);
	static bool MoveRandom(units::Robot& robot);
	static MapLocation PickGreedy(MapLocation& origin, MapLocation& destination);
	static bool MoveGreedy(units::Robot& robot, MapLocation& destination);
	static std::vector<bc_Direction> AStarPath(bc_MapLocation* origin, bc_MapLocation* destination);
};

#endif

