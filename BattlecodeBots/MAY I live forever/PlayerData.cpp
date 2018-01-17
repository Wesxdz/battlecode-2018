#include "PlayerData.h"
#include "GameController.h"
#include "OrbitPattern.h"

#include <iostream>
#include "Utility.h"
#include "Log.h"
#include "MapUtil.h"
#include "PlanetMap.h"

PlayerData::PlayerData()
{

	int period = OrbitPattern::Period();
	int optimalRounds = 749 / period;
	optimalFlightTime = OrbitPattern::Center() - OrbitPattern::Amplitude();
	std::cout << "Optimal Launch Rounds: " << optimalRounds << std::endl;

	for (int i = 1; i <= optimalRounds; i++)
	{
		optimalLaunchRounds.push_back(period * i - period/4);
		std::cout << "Flight time on round " << period * i - period / 4 << ": " << OrbitPattern::Duration(period * i - period / 4) << std::endl;
	}


	///////////////////////////////////////////////

	PlanetMap earth{ GameController::PlanetMap(Earth) };
	for (bc_MapLocation* location : MapUtil::earthLocations) {
		MapLocation deposit{ bc_MapLocation_clone(location) };
		int karb = earth.InitialKarbonite(deposit);
		if (karb > 0) {
			initialKarboniteLocations.push_back(deposit);
			earthStartingKarbonite += karb;
		}
	}
	std::cout << initialKarboniteLocations.size() << " initial Karbonite deposits totaling " << earthStartingKarbonite << "\n";
	CHECK_ERRORS()

	PlanetMap map{ GameController::PlanetMap(Earth) };
	CHECK_ERRORS()

	for (auto& worker : map.InitialWorkers()) {
		if (worker.Team() != GameController::Team()) {
			std::cout << "enemy workers spawn at " <<  worker.Loc().ToMapLocation().X() << ", " <<  worker.Loc().ToMapLocation().Y() << "\n";
			enemySpawnPositions.push_back(worker.Loc().ToMapLocation());
		}
	}

	CHECK_ERRORS()

	////////////////////////////////////////////////



}

void PlayerData::Update()
{
	GatherUnitData();
}

void PlayerData::GatherUnitData()
{
	ClearUnitCounts();
	std::vector<units::Unit> units = GameController::Units(bc_Selection::Visible);
	for (auto unit : units)
	{
		bc_Team uTeam = unit.Team();
		bc_UnitType uType = unit.type;

		uTeam == GameController::Team() ? teamUnitCounts[uType]++ : enemyUnitCounts[uType]++;

	}
}

void PlayerData::ClearUnitCounts()
{
	teamUnitCounts.clear();
	enemyUnitCounts.clear();
}



