#include "PlayerData.h"
#include "GameController.h"
#include "OrbitPattern.h"

#include <iostream>
#include "Utility.h"
#include "Log.h"
#include "MapUtil.h"

PlayerData::PlayerData()
{

	int period = OrbitPattern::Period();
	int optimalRounds = 749 / period;
	optimalFlightTime = OrbitPattern::Center() - OrbitPattern::Amplitude();

	std::cout << "Period: " << period << std::endl;
	std::cout << "Center: " << OrbitPattern::Center() << std::endl;
	std::cout << "Amplitude: " << OrbitPattern::Amplitude() << std::endl;
	std::cout << "Optimal Launch Rounds: " << optimalRounds << std::endl;

	for (int i = 1; i <= optimalRounds; i++)
	{
		optimalLaunchRounds.push_back(period * i - period/4);
		std::cout << "Flight time on round " << period * i - period / 4 << ": " << OrbitPattern::Duration(period * i - period / 4) << std::endl;
	}


	///////////////////////////////////////////////

	for (bc_MapLocation* location : MapUtil::earthLocations) {
		MapLocation loc{ bc_MapLocation_clone(location) };
		std::cout << (bool)loc.IsPassable() << std::endl;
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



