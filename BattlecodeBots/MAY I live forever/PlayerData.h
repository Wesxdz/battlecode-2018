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
	Vec2(int x2, int y2)
	{
		x = x2;
		y = y2;
	}

};

class PlayerData
{
public:
	PlayerData();
	//Updates unit counts and vector of our units.
	void Update();

	std::map<bc_UnitType, int> teamUnitCounts;
	std::map<bc_UnitType, int> enemyUnitCounts;
	bc_PlanetMap* Map(bc_Planet);
	void GatherUnitData();
	void ClearUnitCounts();
	
	int earthStartingKarbonite;
	std::vector<MapLocation> initialKarboniteLocations;
	std::vector<MapLocation> enemySpawnPositions;
	std::vector<MapLocation> passableEarthTerrain;
	std::vector<MapLocation> passableMarsTerrain;

	int optimalLaunchTime;
	std::vector<int> optimalLaunchRounds;

	std::map<bc_UnitType, int> enemyResearchBranches;


};
#endif