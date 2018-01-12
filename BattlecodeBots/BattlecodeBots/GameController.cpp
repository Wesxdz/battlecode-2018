#include "GameController.h"

#include "GameMap.h"
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

bc_PlanetMap* GameController::PlanetMap()
{
	GameMap map;
	if (Planet() == bc_Planet::Earth) {
		return map.Earth().self;
	}
	else {
		return map.Mars().self;
	}
}

bc_Team GameController::Team()
{
	return bc_GameController_team(gc);
}

uint32_t GameController::Karbonite()
{
	return bc_GameController_karbonite(gc);
}

std::shared_ptr<units::Unit> GameController::Unit(uint16_t id)
{
	auto lovely = std::make_shared<units::Unit>();
	lovely->Init(id);
	return lovely;
}

std::shared_ptr<units::Unit> GameController::Unit(bc_Unit* unit)
{
	auto lovely = std::make_shared<units::Unit>();
	lovely->Init(bc_Unit_id(unit));
	return lovely;
}

std::vector<std::shared_ptr<units::Unit>> GameController::Units(bc_Selection selection)
{
	switch (selection) {
	case Visible:
		return Wrap<units::Unit>(bc_GameController_units(gc));
	case MyTeam:
		return Wrap<units::Unit>(bc_GameController_my_units(gc));
	}
	return Wrap<units::Unit>(bc_GameController_units_in_space(gc));
}
