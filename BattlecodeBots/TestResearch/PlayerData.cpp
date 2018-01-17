#include "PlayerData.h"
#include "GameController.h"
#include "OrbitPattern.h"

#include <iostream>
#include "Utility.h"
#include "Log.h"
#include "MapUtil.h"
#include "PlanetMap.h"

PlayerData* PlayerData::pd = nullptr;

PlayerData::PlayerData()
{

	int period = OrbitPattern::Period();
	int optimalRounds = 749 / period;
	optimalFlightTime = OrbitPattern::Center() - OrbitPattern::Amplitude();
	std::cout << "Optimal Launch Rounds: " << optimalRounds << std::endl;
	CHECK_ERRORS();

	for (int i = 1; i <= optimalRounds; i++)
	{
		optimalLaunchRounds.push_back(period * i - period/4);
		std::cout << "Flight time on round " << period * i - period / 4 << ": " << OrbitPattern::Duration(period * i - period / 4) << std::endl;
	}
	CHECK_ERRORS();

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
	CHECK_ERRORS();

	PlanetMap map{ GameController::PlanetMap(Earth) };
	for (auto& worker : map.InitialWorkers()) {
		if (worker.Team() != GameController::Team()) {
			std::cout << "enemy workers spawn at " <<  worker.Loc().ToMapLocation().X() << ", " <<  worker.Loc().ToMapLocation().Y() << "\n";
			enemySpawnPositions.push_back(worker.Loc().ToMapLocation());
		}
	}
	CHECK_ERRORS();

	pd = this;

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
		std::cout << "Unit " << uType << std::endl;
	}
}

void PlayerData::ClearUnitCounts()
{
	teamUnitCounts.clear();
	enemyUnitCounts.clear();
}



