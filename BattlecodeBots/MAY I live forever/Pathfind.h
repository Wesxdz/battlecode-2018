#ifndef PATHFIND_H
#define PATHFIND_H

#include "bc.h"
#include <vector>
#include <functional>
#include "MapLocation.h"
#include "Robot.h"

#include <map>

struct FlowChart {
	short* pointsMap = nullptr;
	bc_Direction* directionMap = nullptr;
	~FlowChart() {
		delete[] pointsMap;
		delete[] directionMap;
	}
};

class Section;

class Pathfind
{
public:
	static std::map<int, FlowChart> flowCharts;
public:
	static std::vector<MapLocation> Neighbors(MapLocation& start);
	static std::vector<MapLocation> Moveable(MapLocation& start);
	static MapLocation PickRandom(std::vector<MapLocation>& locations);
	static bool MoveRandom(units::Robot& robot);
	static bool MoveFuzzy(units::Robot& robot, bc_Direction direction);
	static bool MoveFuzzyFlow(units::Robot& robot, MapLocation& destination);
	static bool MoveFuzzyFlow(units::Robot& robot, int destX, int destY);
	
	static int GetFuzzyFlowTurns(int sourceX, int sourceY, int destX, int destY);
	static int GetFuzzyFlowTurns(MapLocation& origin, MapLocation& destination);

	static FlowChart CreateFlowChart(std::vector<bc_MapLocation*> destinations);
	static FlowChart CreateFlowChart(std::vector<MapLocation> destinations);

private:
	static void Init();
	static void GenerateFlowPath(Section* section, short destX, short destY);
	static void GenerateFlowPathPoints(short* terrainMap, short* pointsMap, Section* section, short destX, short destY);
	static bc_Direction GenerateFlowPathDirection(short* pointsMap, 
		short sourceX, short sourceY, short destX, short DestY);
};

#endif

