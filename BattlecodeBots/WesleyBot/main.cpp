#include <iostream>
#include "GameController.h"
#include "Unit.h"
#include "Worker.h"
#include "Constants.h"

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{
	srand(0);
	GameController player;
	while (true)
	{
		auto units = player.Units(bc_Selection::MyTeam);
		for (auto unit : units) {
			if (unit.type == bc_UnitType::Worker) {
				units::Worker worker{ bc_Unit_clone(unit.self) };
				if (worker.IsMoveReady() && worker.CanMove(Southwest)) {
					worker.Move(Southwest);
				}
				if (!worker.HasActed()) {
					for (bc_Direction direction : constants::directions_adjacent) {
						if (worker.CanBlueprint(Factory, direction)) {
							worker.Blueprint(Factory, direction);
						}
					}
				}
			}
		}
		player.EndTurn();
	}
}