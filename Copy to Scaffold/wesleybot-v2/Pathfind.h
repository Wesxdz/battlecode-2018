#ifndef PATHFIND_H
#define PATHFIND_H

#include "bc.h"
#include <vector>
#include <functional>

class Pathfind
{
public:
	static std::vector<bc_MapLocation*> Neighbors(bc_MapLocation* start);
	static std::vector<bc_MapLocation*> Moveable(bc_MapLocation* start);
	static bc_MapLocation* PickRandom(std::vector<bc_MapLocation*>& locGrid);
	static bool MoveRandom(bc_Unit* robot);
	static bc_MapLocation* PickGreedy(bc_MapLocation* origin, bc_MapLocation* destination);
	static bool MoveGreedy(bc_Unit* robot, bc_MapLocation* destination);
	static std::vector<bc_MapLocation*> AStarPath(bc_MapLocation* origin, bc_MapLocation* destination, std::function<int(bc_MapLocation*)> cost, int effort = 1000);
};

#endif

