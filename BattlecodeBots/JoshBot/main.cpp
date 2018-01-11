#include <iostream>
#include <random>
#include "bc.h"

#include "GameController.h"
#include "Unit.h"
#include "Worker.h"
#include "Location.h"

bool check_errors()
{
	if (bc_has_err())
	{
		char* err;
		int8_t code = bc_get_last_err(&err);
		std::cout << "Engine error code " << code << ": " << err << std::endl;
		bc_free_string(err);
		return true;
	}
	else
	{
		return false;
	}
}

GameController gc;
int round;
std::vector<std::shared_ptr<units::Unit>> units;

enum WorkerPriority {
	KARB,
	FACTORY
};
WorkerPriority currWorkerPriority = WorkerPriority::FACTORY;

void UpdateWorkerEarth(int index) {
	//units::Worker worker;
	//worker.Init(units[index]->self);
	//auto location = worker.Loc();
	//if (location.OnPlanet(bc_Planet::Earth)) {
	//	auto mapLocation = location.ToMapLocation();
	//	if()
	//}
}
void UpdateKnightEarth(int index) {

}
void UpdateRangerEarth(int index) {

}
void UpdateMageEarth(int index) {

}
void UpdateHealerEarth(int index) {
	 
}

void UpdateEarth() {
	std::cout << "This bot is on Earth\n";
	while (true) {
		round = gc.Round();
		std::cout << "Round: " << round;
		// Research...

		units = gc.Units(bc_Selection::MyTeam);
		auto teamCount = units.size();
		for (int i = 0; i < teamCount; i++) {
			
			switch (units[i]->type) {
				case  Worker:
					UpdateWorkerEarth(i);
					break;
				case Knight:
					UpdateKnightEarth(i);
					break;
				case Ranger:
					UpdateRangerEarth(i);
					break;
				case Mage:
					UpdateMageEarth(i);
					break;
				case Healer:
					UpdateHealerEarth(i);
					break;
			}
		}


		gc.EndTurn();
	}
}

void UpdateMars() {
	std::cout << "This bot is on Mars\n";
	while (true) {
		units = gc.Units(bc_Selection::MyTeam);
		if (units.size() > 0) {

		} else {
			gc.EndTurn();
		}
	}
}

int main()
{
	std::cout << "Player C++ bot starting!" << std::endl;
	srand(0);
	check_errors();

	if (gc.Planet() == bc_Planet::Earth) {
		UpdateEarth();
	} else {
		UpdateMars();
	}

	check_errors();
	return 0;



	//while (true)
	//{
	//	uint32_t round = bc_GameController_round(gc);
	//	std::cout << "Round: "<< round << std::endl;

	//	bc_VecUnit *units = bc_GameController_my_units(gc);

	//	int len = bc_VecUnit_len(units);
	//	for (int i = 0; i < len; i++)
	//	{
	//		bc_Unit *unit = bc_VecUnit_index(units, i);

	//		uint16_t id = bc_Unit_id(unit);
	//		if (bc_GameController_can_move(gc, id, North) && bc_GameController_is_move_ready(gc, id)) 
	//		{
	//			bc_GameController_move_robot(gc, id, North);
	//			check_errors();
	//		}
	//		delete_bc_Unit(unit);

	//	}
	//	delete_bc_VecUnit(units);

	//	bc_GameController_next_turn(gc);
	//}
}