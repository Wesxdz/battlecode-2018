#include <iostream>
#include <memory>

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
#include "MapLocation.h"
#include "Location.h"

#include "AStar.h"

GameController gc;
GameMap gameMap;

Research research;
OrbitPattern orbitPattern;
AsteroidPattern asteroidPattern;
TeamArray teamArray;

bc_Team thisTeam = gc.Team();
bc_Planet thisPlanet = gc.Planet();
PlanetMap* thisPlent = gameMap.OurPlanet();

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{

	srand(0);

	std::cout << "A* test initialize" << std::endl;
	GameController player;
	AStar aStar = AStar();
	bool pathed = false;

	if (GameController::Planet() == Mars)
	{
		pathed = true;
	}

	int pathNum = 0;
	while (true)
	{
		uint32_t round = gc.Round();
		std::cout << "Round: " << round << std::endl;
		auto units = player.Units(bc_Selection::MyTeam);
		for (auto unit : units)
		{
			if (!pathed)
			{
				int x = unit.Loc().ToMapLocation().X();
				int y = unit.Loc().ToMapLocation().Y();
				aStar.PathFind(x, y, GameMap::Earth().Width() - x, GameMap::Earth().Height() - y, true);
				pathed = true;
			}
			if (unit.type == bc_UnitType::Worker) {
				units::Worker worker{ bc_Unit_clone(unit.self) };
				if (worker.IsMoveReady() && worker.CanMove(aStar.m_pathDirections[pathNum]))
				{
					worker.Move(aStar.m_pathDirections[pathNum]);
					if (pathNum < aStar.m_pathDirections.size())
						pathNum++;
				}

			}
		}
		gc.EndTurn();
	}
}