#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include "bc.h"
#include <map>
#include <vector>

#include "MapLocation.h"

struct Vec2
{
	int x;
	int y;
};

class PlayerData
{
public:
	PlayerData();

	std::map<bc_UnitType, int> teamUnitCounts;
	std::map<bc_UnitType, int> enemyUnitCounts;
	void ClearUnitCounts();
	
	int earthStartingKarbonite;
	std::vector<Vec2> initialKarbLocations;
	std::vector<Vec2> enemySpawnPositions;

	std::vector<int> optimalLaunchRounds;

	std::vector<units::Unit> teamUnits;

	std::map<bc_UnitType, int> enemyResearchBranches;


};
#endif;