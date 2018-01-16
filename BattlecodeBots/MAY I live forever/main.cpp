#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "GlobalData.h"

#include "Unit.h"
#include "Worker.h"
#include "MapLocation.h"
#include "Location.h"

#include "AStar.h"
#include "Utility.h"

GlobalData data;

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{
	//bc_GameController_get_time_left_ms(); This gets the time remaining supposedly. Header file doesnt have it
	srand(0);

	std::cout << "A* test initialize" << std::endl;

	//AStar aStar = AStar();
	//bool pathed = false;
	AStar aStar = AStar();
	bool pathed = false;

	if (GameController::Planet() == Mars)
	{
		pathed = true;
	}

	int pathNum = 0;
	while (true)
	{
		uint32_t round = GameController::Round();
		std::cout << "Round: " << round << std::endl;
		auto units = GameController::Units(bc_Selection::MyTeam);
		for (auto unit : units)
		{
			if (!pathed)
			{
				int x = unit.Loc().ToMapLocation().X();
				int y = unit.Loc().ToMapLocation().Y();
				//aStar.PathFind(x, y, 10, 10, true);
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
		GameController::EndTurn();
	}
}