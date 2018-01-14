#ifndef GAMEINFO_H
#define GAMEINFO_H

#include "bc.h"

#include "Unit.h"
#include "Worker.h"
#include "Knight.h"
#include "Ranger.h"
#include "Mage.h"
#include "Healer.h"
#include "Factory.h"
#include "Rocket.h"
#include "PlanetMap.h"

#include <vector>
#include <memory>

struct GameInfo {
	std::vector<std::shared_ptr<units::Unit>> currEnemys;
	std::vector<std::shared_ptr<units::Unit>> currUnits;
	std::vector<std::shared_ptr<units::Worker>> currWorkers;
	std::vector<std::shared_ptr<units::Knight>> currKnights;
	std::vector<std::shared_ptr<units::Ranger>> currRangers;
	std::vector<std::shared_ptr<units::Mage>> currMages;
	std::vector<std::shared_ptr<units::Healer>> currHealers;
	std::vector<std::shared_ptr<units::Factory>> currFactorys;
	std::vector<std::shared_ptr<units::Rocket>> currRockets;

	MapLocation* enemySpawns;

	PlanetMap planetMap;

	bc_Team thisTeam;

	int currRound;
	int currKarbonite;
	int enemySpawnAmo;
};
#endif // !GAMEINFO_H