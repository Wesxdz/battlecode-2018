#include "GameController.h"

#include "Unit.h"
#include "Knight.h"
#include "Worker.h"
#include "Ranger.h"
#include "Mage.h"
#include "Healer.h"
#include "Factory.h"
#include "Rocket.h"

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
	return Unit(bc_GameController_unit(gc, id));
}

std::shared_ptr<units::Unit> GameController::Unit(bc_Unit* unit)
{
	bc_UnitType type = bc_Unit_unit_type(unit);
	if (type == Worker) {
		auto worker = std::make_shared<units::Worker>();
		worker->Init(unit);
		return worker;
	}
	else if (type == Knight) {
		auto knight = std::make_shared<units::Knight>();
		knight->Init(unit);
		return knight;
	}
	else if (type == Ranger) {
		auto ranger = std::make_shared<units::Ranger>();
		ranger->Init(unit);
		return ranger;
	}
	else if (type == Mage) {
		auto mage = std::make_shared<units::Mage>();
		mage->Init(unit);
		return mage;
	}
	else if (type == Healer) {
		auto healer = std::make_shared<units::Healer>();
		healer->Init(unit);
		return healer;
	}
	else if (type == Factory) {
		auto factory = std::make_shared<units::Factory>();
		factory->Init(unit);
		return factory;
	}
	else {
		auto rocket = std::make_shared<units::Rocket>();
		rocket->Init(unit);
		return rocket;
	}
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
