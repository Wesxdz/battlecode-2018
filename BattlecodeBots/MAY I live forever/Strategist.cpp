#include "Strategist.h"

#include "bc.h"
#include "GameController.h"
#include "PlayerData.h"
#include "Section.h"
#include "Constants.h"
#include <iostream>

Strategy Strategist::strategy;

Strategist::Strategist()
{
	// By default, choose AntiRush
	strategy = WizardWin;

	// Choose Psychonaut if we start on entirely different sections
	int sharedSections = 0;
	for (Section* section : Section::earthSections) {
		if (section->status == StartStatus::Mixed) sharedSections++;
	}
	if (sharedSections == 0) {
		strategy = Psychonaut;
	}
	else {
		// Choose TerroristOvercharge if the starting sections have lots of impassable terrain that blocks flows to enemies
		// We determine this by checking the number of 4-way impassable tiles next to every section location
		int passableAdjacent = 0;
		int impassableAdjacent = 0;
		for (Section* section : Section::earthSections) {
			if (section->status == StartStatus::Mixed) {
				for (bc_MapLocation* loc : section->locations) {
					MapLocation location = bc_MapLocation_clone(loc);
					for (bc_Direction direction : constants::directions_adjacent) {
						MapLocation neighbor = MapLocation::Neighbor(location, direction);
						if (neighbor.IsValid() && neighbor.IsPassable()) {
							passableAdjacent++;
						}
						else {
							impassableAdjacent++;
						}
					}
				}
			}
		}
		if (impassableAdjacent > passableAdjacent / 6) {
			strategy = TerroristOvercharge;
		}
	}

	std::cout << "Strategy chosen: ";
	if (strategy == Psychonaut) {
		std::cout << "Psychonaut" << std::endl;
	}
	else if (strategy == WizardWin) {
		std::cout << "WizardWin" << std::endl;
	}
	else if (strategy == TerroristOvercharge) {
		std::cout << "TerroristOvercharge" << std::endl;
	}

	if (strategy == Psychonaut) {

		bc_GameController_queue_research(GameController::gc, Rocket); // 50
		bc_GameController_queue_research(GameController::gc, Worker); // +25 = 75
		bc_GameController_queue_research(GameController::gc, Ranger); // +25 = 100
		bc_GameController_queue_research(GameController::gc, Ranger); // +100 = 200
		bc_GameController_queue_research(GameController::gc, Ranger); // +200 = 400
		bc_GameController_queue_research(GameController::gc, Worker); // +75 = 475
		bc_GameController_queue_research(GameController::gc, Worker); // +75 = 550
		bc_GameController_queue_research(GameController::gc, Worker); // +75 = 625
		bc_GameController_queue_research(GameController::gc, Healer); // +25 = 650
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 750
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 850
		// ???

	}
	else if (strategy == WizardWin) {

		bc_GameController_queue_research(GameController::gc, Ranger); // +100 = 325
		bc_GameController_queue_research(GameController::gc, Ranger); // +100 = 325
		bc_GameController_queue_research(GameController::gc, Mage); // 25
		bc_GameController_queue_research(GameController::gc, Mage); // +75 = 100
		bc_GameController_queue_research(GameController::gc, Mage); // +100 = 200
		bc_GameController_queue_research(GameController::gc, Mage); // +25 = 225
		bc_GameController_queue_research(GameController::gc, Rocket); // +50 = 475
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 425
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 425
		bc_GameController_queue_research(GameController::gc, Rocket); // +100 = 575
		bc_GameController_queue_research(GameController::gc, Rocket); // +100 = 675
		bc_GameController_queue_research(GameController::gc, Ranger); // +25 = 975

	}
	else if (strategy == TerroristOvercharge) {

		bc_GameController_queue_research(GameController::gc, Healer); // 25
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 125
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 225
		bc_GameController_queue_research(GameController::gc, Mage); // +25 = 250
		bc_GameController_queue_research(GameController::gc, Mage); // +75 = 325
		bc_GameController_queue_research(GameController::gc, Mage); // +100 = 425
		bc_GameController_queue_research(GameController::gc, Mage); // +75 = 500
		bc_GameController_queue_research(GameController::gc, Rocket); // +50 = 550
		bc_GameController_queue_research(GameController::gc, Ranger); // +25 = 575
		bc_GameController_queue_research(GameController::gc, Ranger); // +100 = 675
		bc_GameController_queue_research(GameController::gc, Ranger); // +200 = 875
		bc_GameController_queue_research(GameController::gc, Knight); // +25 = 900
		bc_GameController_queue_research(GameController::gc, Knight); // +75 = 975

	}

}

Strategist::~Strategist()
{
}
