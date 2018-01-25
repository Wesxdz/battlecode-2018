#include "Strategist.h"

#include "bc.h"
#include "GameController.h"

Strategist::Strategist()
{
	strategy = ShieldFormation;

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
	else if (strategy == ShieldFormation) {

		bc_GameController_queue_research(GameController::gc, Knight); // 25
		bc_GameController_queue_research(GameController::gc, Knight); // +75 = 100
		bc_GameController_queue_research(GameController::gc, Knight); // +100 = 200
		bc_GameController_queue_research(GameController::gc, Healer); // +25 = 225
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 325
		bc_GameController_queue_research(GameController::gc, Healer); // +100 = 425
		bc_GameController_queue_research(GameController::gc, Rocket); // +50 = 475
		bc_GameController_queue_research(GameController::gc, Rocket); // +100 = 575
		bc_GameController_queue_research(GameController::gc, Rocket); // +100 = 675
		bc_GameController_queue_research(GameController::gc, Mage); // +25 = 700
		bc_GameController_queue_research(GameController::gc, Mage); // +75 = 775
		bc_GameController_queue_research(GameController::gc, Mage); // +100 = 875
		bc_GameController_queue_research(GameController::gc, Mage); // +75 = 950
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
