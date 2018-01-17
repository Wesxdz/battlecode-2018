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
// Static Variables. Should only be created once
	GameController gc;
	GameMap gameMap;

	Research research;
	OrbitPattern orbitPattern;
	AsteroidPattern asteroidPattern;
	RocketLandingInfo rocketLandingInfo;
	TeamArray teamArray;

// Variables that will be updated every frame
	std::vector<units::Unit> currEnemys;

	std::vector<units::Knight> currEnemyKnights;
	std::vector<units::Ranger> currEnemyRangers;
	std::vector<units::Mage> currEnemyMages;
	std::vector<units::Healer> currEnemyHealers;
	std::vector<units::Unit> currEnemyOthers;

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

// Initia; variables set when the game begins
	std::vector<MapLocation> passableEarthTerrain;
	std::vector<MapLocation> passableMarsTerrain;

	std::vector<MapLocation> initialKarbLocations;
	int totalInitialKarb = 0;

	int minLaunchTurns;
	int launchStart;
	int launchFrequency;

	MapLocation* enemySpawns;
	int enemySpawnAmo;

// Useful variables
	PlanetMap* thisPlanetMap;
	bc_Team thisTeam;
	int currKarbonite;
	int currRound;

	static GlobalData* data;
public:
	GlobalData();
	~GlobalData();

	void Init();
	void Update();
};

#endif

