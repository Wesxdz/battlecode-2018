#include <stdlib.h>
#include "bc.h"
#include "Player.h"
#include "Pathfind.h"
#include "Section.h"
#include <iostream>
#include "UnitPolicies.h"
#include "Policy.h"
#include <algorithm>
#include "Log.h"
#include "Info.h"
#include <chrono>
#include <ctime>
#include "ResearchPolicies.h"

int main()
{
	srand(0);
	Player player;
	bc_GameController* gc = Player::gc;
	Info info;
	UnitPolicies actions;
	actions.Init();
	ResearchPolicies research;
	if (bc_GameController_planet(gc) == Earth) {
		research.Init();
	}
	std::chrono::duration<double> totalTime;
	while (true)
	{
		auto start = std::chrono::system_clock::now();
		uint32_t round = bc_GameController_round(gc);
		//std::cout << "--Round: " << round << "\n";
		//std::cout << "Karbonite: " << bc_GameController_karbonite(gc) << "\n";
		info.Update();
		if (bc_GameController_planet(gc) == Earth) {
			research.Update();
		}
		bc_VecUnit* units = bc_GameController_my_units(gc);
		for (uintptr_t i = 0; i < bc_VecUnit_len(units); i++) {
			bc_Unit* unit = bc_VecUnit_index(units, i);
			uint16_t id = bc_Unit_id(unit);
			bc_UnitType type = bc_Unit_unit_type(unit);
			actions.SetActive(unit);
			if (actions.policies[type].size() > 0) {
				auto it = std::max_element(actions.policies[type].begin(), actions.policies[type].end(), [](std::shared_ptr<Policy> a, std::shared_ptr<Policy> b) {
					return a->Evaluate() < b->Evaluate();
				});
				std::shared_ptr<Policy> chosenPolicy = *it;
				if (chosenPolicy->Evaluate() > 0) { // TODO Store to increase performance
					(*it)->Command();
				}
			}
			delete_bc_Unit(unit);
		}
		delete_bc_VecUnit(units);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> roundTime = end - start;
		totalTime += roundTime;
		//std::cout << "Total time used: " << totalTime.count() << "\nRound time used: " << roundTime.count() << "\n";
		bc_GameController_next_turn(gc);
	}
}