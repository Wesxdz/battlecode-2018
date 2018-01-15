#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include "bc.h"

#include "GameController.h"
#include "GameMap.h"
#include "PlanetMap.h"
#include "Research.h"
#include "OrbitPattern.h"
#include "AsteroidPattern.h"
#include "TeamArray.h"

#include "Unit.h"
#include "Worker.h"
#include "Knight.h"
#include "Ranger.h"
#include "Mage.h"
#include "Healer.h"
#include "Factory.h"
#include "Rocket.h"

#include <vector>
#include <memory>

class GlobalData
{
public:
	GameController gc;
	GameMap gameMap;

	Research research;
	OrbitPattern orbitPattern;
	AsteroidPattern asteroidPattern;
	TeamArray teamArray;

	std::vector<units::Unit> currEnemys;

	std::vector<units::Knight> currEnemyKnights;
	std::vector<units::Ranger> currEnemyRangers;
	std::vector<units::Mage> currEnemyMages;
	std::vector<units::Healer> currEnemyHealers;

	std::vector<units::Unit> currUnits;
	static std::vector<units::Robot> currRobots;

	std::vector<units::Worker> currWorkers;
	std::vector<units::Knight> currKnights;
	std::vector<units::Ranger> currRangers;
	std::vector<units::Mage> currMages;
	std::vector<units::Healer> currHealers;
	std::vector<units::Factory> currFactorys;
	std::vector<units::Rocket> currRockets;

	std::vector<units::Factory> unBuiltFactorys;
	std::vector<units::Rocket> unBuiltRockets;

	std::vector<MapLocation> passableEarthTerrain;
	std::vector<MapLocation> passableMarsTerrain;

	std::vector<MapLocation> initialKarbLocations;
	int totalInitialKarb = 0;

	int minLaunchTurns;
	int launchStart;
	int launchFrequency;

	MapLocation* enemySpawns;
	int enemySpawnAmo;

	PlanetMap* thisPlanetMap = gameMap.OurPlanet();
	bc_Team thisTeam = gc.Team();
};

#endif

