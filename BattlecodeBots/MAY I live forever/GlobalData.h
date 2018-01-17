#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include "bc.h"

#include "GameController.h"
#include "PlanetMap.h"
#include "Research.h"
#include "OrbitPattern.h"
#include "AsteroidPattern.h"
#include "TeamArray.h"
#include "RocketLandingInfo.h"

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
#include <map>

class GlobalData
{
public:
	GameController gc;

	Research research;
	OrbitPattern orbitPattern;
	AsteroidPattern asteroidPattern;
	RocketLandingInfo rocketLandingInfo;
	TeamArray teamArray;

	std::vector<units::Unit> currEnemys;

	std::vector<units::Knight> currEnemyKnights;
	std::vector<units::Ranger> currEnemyRangers;
	std::vector<units::Mage> currEnemyMages;
	std::vector<units::Healer> currEnemyHealers;

	std::vector<units::Unit> currUnits;
	std::vector<units::Robot> currRobots;

	std::vector<units::Worker> currWorkers;
	std::vector<units::Knight> currKnights;
	std::vector<units::Ranger> currRangers;
	std::vector<units::Mage> currMages;
	std::vector<units::Healer> currHealers;

	

	//This data goes in the builder overlord, as soon as you dont need it please remove it - Evan
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

	bc_Team thisTeam = gc.Team();
};

#endif

