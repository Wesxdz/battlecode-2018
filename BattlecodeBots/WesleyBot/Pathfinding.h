#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include <functional>
#include <map>
#include "MapLocation.h"

class Pathfinding
{
public:
	/*
	@return Valid adjacent locations
	*/
	static std::vector<MapLocation> Neighbors(MapLocation& start);
	/*
	@return Adjacent locations that can be moved to
	*/
	static std::vector<MapLocation> Moveable(MapLocation& start);
	/*
	@return Nearby deposits that can be mined from!
	*/
	static std::vector<MapLocation> Deposits(MapLocation& start);
	/*
	@precondition locations.size() > 0
	@return A random location from #locations
	*/
	static MapLocation PickRandom(std::vector<MapLocation>& locations);
	/*
	@note Returns start if there are no adjacent places to move
	@return A 
	*/
	static bc_Direction PickGreedy(MapLocation& start, MapLocation& end);
	static MapLocation PickGreedyFancy(MapLocation start, MapLocation end, std::map<int, MapLocation>* discouraged = nullptr);
	/*
	Uses A* search algorithm
	@param cost Evaluates the cost of traveling through a particular MapLocation
	@return A path from #start to #end (including both)
	*/
	static std::vector<MapLocation> FindPath(MapLocation start, MapLocation end, std::function<int(MapLocation)> cost);
};

#endif

