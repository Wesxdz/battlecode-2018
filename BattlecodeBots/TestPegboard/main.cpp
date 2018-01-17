#include <iostream>
#include "GameController.h"
#include "Unit.h"
#include "Worker.h"
#include "Constants.h"
#include "Pegboard.h"

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
		if (GameController::Planet() == Earth) {
			units::Unit unit{ units[0] };
			if (unit.type == bc_UnitType::Worker) {
				units::Worker worker{ bc_Unit_clone(unit.self) };
				if (worker.IsMoveReady()) {
					Pegboard::ApplyForce(worker.id, East, 1.0f);
					Pegboard::ApplyForce(worker.id, East, .2f);
				}
				//if (!worker.HasActed()) {
				//	for (bc_Direction direction : constants::directions_adjacent) {
				//		if (worker.CanReplicate(direction)) {
				//			worker.Replicate(direction);
				//		}
				//	}
				//}
			}
			//for (auto& unit : units) {
			//}
			Pegboard::SolveMovement();
		}
		player.EndTurn();
	}
}