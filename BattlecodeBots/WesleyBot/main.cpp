#include "GameController.h"
#include "Unit.h"
#include "Worker.h"
#include <iostream>

int main() 
{
	std::cout << "WesleyBot initialize" << std::endl;
	GameController player;
	while (true) {
		auto units = player.Units(bc_Selection::MyTeam);
		for (auto unit : units) {
			if (unit->type == bc_UnitType::Worker) {
				auto worker = player.Convert<units::Worker>(unit);
				if (worker->IsMoveReady() && worker->CanMove(North)) {
					worker->Move(North);
				}
			}
		}
		player.EndTurn();
	}
}