#ifndef PATHFIND_H
#define PATHFIND_H

#include "bc.h"
#include <vector>
#include <functional>
#include "MapLocation.h"
#include "Robot.h"

#include <map>

struct FlowChart {
	short* pointsMap;
	bc_Direction* directionMap;
};

class Pathfind
{
public:
	static short* terrainMap;
	static std::map<int, FlowChart> flowCharts;
public:
	static std::vector<MapLocation> Neighbors(MapLocation& start);
	static std::vector<MapLocation> Moveable(MapLocation& start);
	static MapLocation PickRandom(std::vector<MapLocation>& locations);
	static bool MoveRandom(units::Robot& robot);
	static bool MoveFuzzy(units::Robot& robot, bc_Direction direction);
	static bool MoveFuzzyFlow(units::Robot& robot, MapLocation& destination);
	static bool MoveFuzzyFlow(units::Robot& robot, int destX, int destY);

private:
	static short GenerateFlowPathTerrain(short ID, bc_PlanetMap* plantMapPtr);
	static void GenerateFlowPathPoints(short* terrainMap, short* pointsMap, short destX, short destY);
	static bc_Direction GenerateFlowPathDirection(short* pointsMap, 
		short sourceX, short sourceY, short destX, short DestY);
};

#endif

