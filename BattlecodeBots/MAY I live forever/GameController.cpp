#include "GameController.h"

#include "PlanetMap.h"
#include "Unit.h"
#include "Knight.h"
#include "Worker.h"
#include "Ranger.h"
#include "Mage.h"
#include "Healer.h"
#include "Factory.h"
#include "Rocket.h"


bc_GameController* GameController::gc = nullptr;

GameController::GameController()
{
	gc = new_bc_GameController();
}

GameController::~GameController()
{
	delete_bc_GameController(gc);
}

void GameController::EndTurn()
{
	bc_GameController_next_turn(gc);
}

uint32_t GameController::Round()
{
	return bc_GameController_round(gc);
}

bc_Planet GameController::Planet()
{
	return bc_GameController_planet(gc);
}

bc_PlanetMap * GameController::PlanetMap(bc_Planet planet)
{
	if (planet == Earth) return  bc_GameController_starting_map(GameController::gc, bc_Planet::Earth);
	return bc_GameController_starting_map(GameController::gc, bc_Planet::Mars);
}

bc_Team GameController::Team()
{
	return bc_GameController_team(gc);
}

uint32_t GameController::Karbonite()
{
	return bc_GameController_karbonite(gc);
}

std::vector<units::Unit> GameController::Units(bc_Selection selection)
{
	std::vector<units::Unit> selected;
	bc_VecUnit* units = nullptr;
	switch (selection) {
	case Visible:
		units = bc_GameController_units(gc);
		break;
	case MyTeam:
		units = bc_GameController_my_units(gc);
		break;
	default:
		units = bc_GameController_units_in_space(gc);
	}
	for (uintptr_t i = 0; i < bc_VecUnit_len(units); i++) {
		selected.push_back(bc_VecUnit_index(units, i));
	}
	delete_bc_VecUnit(units);
	return selected;
}
