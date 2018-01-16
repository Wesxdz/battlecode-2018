#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "PlayerData.h"
#include "GameController.h"

#include "Unit.h"
#include "Worker.h"
#include "MapLocation.h"
#include "Location.h"

#include "BuilderOverlord.h"

#include "Utility.h"

BuilderOverlord builder;
PlayerData playerData;

int main()
{
	srand(0);
	std::cout << "Builder Overlord test initialize" << std::endl;

	while (true)
	{
		uint32_t round = GameController::Round();
		std::cout << "Round: " << round << std::endl;

		GatherUnitData();

		



		GameController::EndTurn();
	}
}

//Count units and unit types for both teams, doesn't count units in space. (Doesn't belong in main either tbh)
void GatherUnitData()
{
	playerData.ClearUnitCounts();
	std::vector<units::Unit> units = GameController::Units(bc_Selection::Visible);
	for (auto unit : units)
	{
		bc_Team uTeam = unit.Team();
		bc_UnitType uType = unit.type;

		uTeam == GameController::Team() ? playerData.teamUnitCounts[uType]++ : playerData.enemyUnitCounts[uType]++;

	}
}

//Loop through my units and tell the correct overlord that they exist.
void UpdateUnits()
{
	std::vector<units::Unit> myUnits = GameController::Units(bc_Selection::MyTeam);
	for (auto unit : myUnits)
	{
		switch (unit.type)
		{
		case 0:
		case 5:
		case 6:
			builder.AddToUnitContainers(unit.type, unit.id);
			break;

		case 1:
		case 2:
		case 3:
		case 4:
			//Unit is a combat unit.
			break;
		}
	}

}