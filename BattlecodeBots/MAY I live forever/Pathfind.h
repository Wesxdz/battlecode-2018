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
	static bool MoveFuzzy(units::Robot& robot, bc_Direction direction);
};

#endif

