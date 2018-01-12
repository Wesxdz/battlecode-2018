#include <stdlib.h>
#include "bc.h"
#include "Player.h"
#include "Pathfind.h"
#include "Section.h"
#include <iostream>

int main() 
{
	srand(0);
	Player::Init();
	bc_GameController* gc = Player::gc;

	bc_MapLocation* test = new_bc_MapLocation(Earth, 10, 10);
	auto sections = Section::GenSections(Player::mars);
	std::cout << "There are " << sections.size() << " sections on Mars!\n";
	while (true)
	{
		uint32_t round = bc_GameController_round(gc);
		std::cout << "--Round: " << round << "\n";
		std::cout << "Karbonite: " << bc_GameController_karbonite(gc) << "\n";
		bc_VecUnit* units = bc_GameController_my_units(gc);
		for (uintptr_t i = 0; i < bc_VecUnit_len(units); i++) {
			bc_Unit* unit = bc_VecUnit_index(units, i);
			uint16_t id = bc_Unit_id(unit);
			if (bc_Unit_unit_type(unit) == Worker) {
				bc_Location* location = bc_Unit_location(unit);
				if (bc_Location_is_on_map(location)) {
					bc_MapLocation* mapLocation = bc_Location_map_location(location);
					//Pathfind::MoveGreedy(unit, test);
					Pathfind::MoveRandom(unit);
					if (bc_GameController_can_harvest(gc, id, bc_Direction::Center)) {
						bc_GameController_harvest(gc, id, bc_Direction::Center);
					}
					delete_bc_MapLocation(mapLocation);
				}
				delete_bc_Location(location);
			}
			delete_bc_Unit(unit);
		}
		delete_bc_VecUnit(units);
		bc_GameController_next_turn(gc);
	}
	delete_bc_MapLocation(test);
	Player::Shutdown();
}